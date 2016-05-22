// ---------------------------------------------------------------------------
//  Includes
// ---------------------------------------------------------------------------
#include "CodeGenerator.hpp"

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>

#include <libxml/tree.h>
#include <libxml/parser.h>

#define HEAD 0
#define CPP 1

using namespace std;

xmlNode *gRootNode = NULL;

static void usage()
{
    cout << "\nUsage:\n"
        "    cg [options] <XML file | List file>\n\n"
        "Options:\n"
        "    -h          Show this help.\n"
        "    -t=xxx      Type of generated file [hpp | cpp*].\n\n"
        "  * = Default if not provided explicitly.\n"
         << endl;
}


/**
 * linear search the attributes list to
 * find the one whose name is n;
 * @attr : the list of attributes
 * @n : the expected name
 * @return : NULL/the value
 */
static const char* matchAttribute(xmlAttr* attr, const char* n) 
{
    if (!attr) return NULL;
    for (xmlAttr* cur = attr; cur; cur = cur->next){
        if (strcmp((const char*)cur->name,n) == 0) {
            xmlNode* value = cur->children;
            assert(value->type == XML_TEXT_NODE);
            return (const char*) value->content;
        }
    }
    return NULL;
}


/**
 * search the children of gRootNode whose id is n
 * return the first matched child 
 */
static xmlNode* searchNodeById(const char *n)
{
    if (n == NULL) return NULL;
    
    xmlNode *child;
    for (child = gRootNode->children; child; child=child->next) {
        if (child->type == XML_ELEMENT_NODE &&
            (strcmp(n,matchAttribute(child->properties,"id")) == 0))
            return child;
    }
    return NULL;
}

// ---------------------------------------------------------------------------
// given the DOMElement n,
// return the Tag of the node
// ---------------------------------------------------------------------------
static const char* getTag(xmlNode* n)
{
    return  (const char*)(n->name);
}


/**
 * given node and attribute name; return attribute value
 * if not found, return NULL
 * @n : the xml element node
 * @a_name : the expected attribute name
 * @return : the attribute value / NULL if not found
 */
static const char* getAttribute(xmlNode* n, const char* a_name)
{  
    return matchAttribute(n->properties,a_name);
}


/**
 * give the type node id, return the complete type name
 * when the type is pointer or array, the function will be called recursively
 * @asRef means this type is used as a reference 
 */
static string getTypeName(const char* tid, bool asRef=false, bool asConst=false)
{
    xmlNode *n = NULL;
    string tcat,tname;
    char len[10];

    n = searchNodeById(tid);
    if (n == NULL) return NULL;
  
    tcat = getTag(n);
    if (tcat == "FundamentalType" ) 
        return "FundamentalType_tmpl<" + string(getAttribute(n,"name")) + ">";
    
    if (tcat == "PointerType")
        return "PointerType_tmpl<" + getTypeName(getAttribute(n,"type")) + ">";
    
    if (tcat == "ArrayType") {
        sprintf(len, "%d", atoi(getAttribute(n,"max")) + 1);
        return "ArrayType_tmpl<" +  getTypeName(getAttribute(n,"type")) +"," + len +  ">";
    }

    if (tcat == "ReferenceType") {
        return getTypeName(getAttribute(n,"type"),true);
    }

    if (tcat == "CvQualifiedType") {
        return getTypeName(getAttribute(n,"type"),asRef,true);
    }

    if (tcat == "Class" ||tcat == "Typedef" || tcat == "Struct") {

        // find the namespace;
        xmlNode *context = searchNodeById(getAttribute(n,"context"));
        assert(context != NULL);
        string ns = getAttribute(context,"name");
        if (ns == "::")
            ns = "";
        else
            ns = ns + "::";

        if (ns == "")
            return "ClassType_tmpl<" + string(asConst?"const ":"") 
                + string(getAttribute(n,"name")) + string(asRef?"&":"") + ">";
        else
            //for the class defined by the 3rd party like std 
            return "FundamentalType_tmpl<" + string(asConst?"const ":"") + ns + 
                string(getAttribute(n,"name")) + string(asRef?"&":"") + ">";
    }
    
    printf("unsupported type defined in this namespace : %s\n", tcat.c_str());
    assert(false);    
    return "";
}

/**
 * given a node whose tag is <Field ...>
 * build a Xct_Field structure for convinience;
 * @n , the xml node whose tag is <Field...>
 * @return : the built Xct_Field structure;
 */
static Xct_Field* buildField(xmlNode* n) 
{
    Xct_Field *f = new Xct_Field();
    f->name = string(getAttribute(n,"name"));
    const char* ch = getAttribute(n,"access");
    f->access = string(ch?ch:"");
    f->type = getTypeName(getAttribute(n,"type"));
    return f;
}

/**
 * given a node whose tag is <Method ...>
 * build a Xct_Method structure for convinience;
 * @n , the xml node whose tag is <Method...>
 * @return : the built Xct_Method structure;
 */
static Xct_Method* buildMethod(xmlNode* node) 
{
    Xct_Method *m = new Xct_Method();
    m->name = string(getAttribute(node,"name"));
    const char* ch = getAttribute(node,"access");
    m->access = string(ch?ch:"");
    // return type
    m->ret_type = getTypeName(getAttribute(node,"returns"));
    // arguments
    if (node->children) {
        for (xmlNode* n = node->children; n; n=n->next) {
            if (strcmp(getTag(n),"Argument") == 0)
                m->arg_types.push_back(getTypeName(getAttribute(n,"type")));
        }
    }
    return m;
}

/**
 * given a node whose tag is <Class ...>
 * build a Xct_Class structure for convinience;
 * @n , the xml node whose tag is <Class...>
 * @return : the built Xct_Class structure;
 */
static Xct_Class* buildClass(xmlNode* n) 
{
    Xct_Class *c = new Xct_Class();
    c->name = getAttribute(n,"name");

    char* _id;
    xmlNode* tmp_n;
    if ((_id= const_cast<char*>(getAttribute(n,"bases"))) != NULL){
        _id = strtok(_id," "); //a bug of gccxml,there is a extra " " at end of _id
        if ((tmp_n = searchNodeById(_id)) != NULL)
            c->baseClass = getAttribute(tmp_n,"name");
    }

    char* _members;
    
    _members = const_cast<char*>(getAttribute(n,"members"));
    _id = strtok(_members, " ");
    while (_id != NULL) {
        if ((tmp_n = searchNodeById(_id)) != NULL) {
            if (strcmp(getTag(tmp_n), "Field") == 0) {
                c->fields.push_back(buildField(tmp_n));
            }
            else if (strcmp(getTag(tmp_n), "Method") == 0) {
                c->methods.push_back(buildMethod(tmp_n));
            }
        }
        _id = strtok(NULL, " ");
    }
    return c;
}

/**
 * given a method, generate teh declartion statement as below
 * MemberFunctionDcl001<D, FundamentalType_tmpl<void>, FundamentalType_tmpl<int>, &D::m1_void, D_strings::m_m1_void>
 * 
 */
static std::string
getMemberFunctionDcl(const string& className, Xct_Method* m) 
{
    stringstream ss;

    // MemberFunctionDcl00x
    ss << "MemberFunctionDcl" << std::setw(3) << std::setfill('0') << m->arg_types.size();
    // <D
    ss << "<" + className;
    // , return_type
    ss << ", " +  m->ret_type;

    // , arg_type
    for (vector<string>::iterator it = m->arg_types.begin();
         it != m->arg_types.end(); ++it) 
        ss << ","  + *it;

    // , &D::m1, D_strings::m_m1
    ss <<", &" + className + "::" + m->name + ", " + className +  "_strings::m_" + m->name;
    // ">"
    ss << ">";
    
    return ss.str();
}


/**
 * given the Xct_Class
 * generate the Head file for the class
 */
static void genHpp(Xct_Class* c)
{
    vector<Xct_Field*>::const_iterator p;
    cout << "#ifndef CPP_REFLECT_" + c->name + "_H\n"
         << "#define CPP_REFLECT_" + c->name + "_H\n\n\n"
         << "#include <reflcpp/ArrayType.hpp>\n"
         << "#include <reflcpp/ClassType_tmpl.hpp>\n"
         << "#include <reflcpp/FundamentalType.hpp>\n"
         << "#include <reflcpp/PointerType.hpp>\n\n\n"
         << "#include <reflcpp/MemberFunctionDcl.hpp>\n"
         << "#include \"" + c->name + ".hpp\"\n";

    if (!c->baseClass.empty())
        cout << "#include \"" + c->baseClass + "_reflection.hpp\"\n";
  
    //generator the #include class.hpp
    int sp,ep;
    for (p = c->fields.begin(); p != c->fields.end(); ++p)
        if ((sp = (*p)->type.find("ClassType_tmpl<")) != string::npos){
            sp = sp + strlen("ClassType_tmpl<") - 1;
            ep = sp + 1;
            while ((*p)->type[ep] != '>') ep++;
            cout << "#include \"" +  (*p)->type.substr(sp + 1, ep - sp - 1) + "_reflection.hpp\"\n";
        }

    //generate #include for classes of method arguments
    vector<Xct_Method*>::const_iterator method_it;
    vector<string>::const_iterator arg_it;
    for (method_it = c->methods.begin(); method_it != c->methods.end(); ++method_it) {
        const std::vector<std::string> &arg_types = (*method_it)->arg_types;
        for (arg_it = arg_types.begin(); arg_it != arg_types.end(); ++arg_it) {
            string argName = *arg_it;
            if ((sp = argName.find("ClassType_tmpl<")) != string::npos){
                sp = sp + strlen("ClassType_tmpl<") - 1;
                ep = sp + 1;
                while ((argName[ep] != '>') && (argName[ep] != '&')) ep++;
                cout << "#include \"" +  argName.substr(sp + 1, ep - sp - 1) + "_reflection.hpp\"\n";
            }
        }
    }
 
    cout << "namespace reflcpp {\n\n"
         << "template <typename Bottom_TP, typename Der_TP, int N>\n"
         << "class Bases<Bottom_TP, Der_TP, " + c->name + ", N> : public BaseList<Bottom_TP, " 
         <<  c->name;
    if (!c->baseClass.empty())
        cout << "," + c->baseClass;
    cout << " > {};\n"
         << "\n\nstruct " + c->name + "_strings {\n"
         << "\tstatic const char name[];\n";
  
    for (p = c->fields.begin(); p != c->fields.end(); ++p)
        cout << "\tstatic const char f_" +  (*p)->name + "[];\n";

    vector<Xct_Method*>::const_iterator m;
    for (m = c->methods.begin(); m != c->methods.end(); ++m) 
        cout << "\tstatic const char m_" +  (*m)->name + "[];\n";
    
    cout << "};\n"
         << "\ntemplate <>\n"
         << "class Members <" + c->name + ">\n"
         << " : public MemberList<\n\t" + c->name + ",\n"
         << "\t" + c->name + "_strings::name\n";

    for (p = c->fields.begin(); p != c->fields.end(); ++p) {
        // Only public fields can be accessible
        if ((*p)->access == "") {
            cout << "\t, DataMemberDcl<" + c->name + "," + (*p)->type 
                 << ", &" + c->name + "::" + (*p)->name + ", " + c->name + "_strings::f_"+ (*p)->name + ">\n";
        }
    }

    for (m = c->methods.begin(); m != c->methods.end(); ++m) {
        // Only public methods can be accessible
        if ((*m)->access == "") {
            cout << "\t," << getMemberFunctionDcl(c->name,*m) << "\n";
        }
    }

    cout << "\t>\n{};\n\n}\n\n#endif\n";
}



       

/**
 * given the id of the class
 * generate the Head file for the class
 */
static void genHpp(char* cid)
{
    xmlNode *n = searchNodeById(cid);
    if (n == NULL) {
        cerr << "Can't find the node with id " << cid << endl;
        return;
    }
    
    if (strcmp(getTag(n),"Class") != 0) {
        cerr << "non-Class definiation found" << endl
             << "You may need to run gccxml with -fxml-start option" << endl;
        return;
    }
    
    Xct_Class* c = buildClass(n);
    genHpp(c);

    // FIXME, the memory leaking for methods and fields
    delete c;
}


static void genCpp (Xct_Class* c) 
{
    string className =  c->name;

    fprintf(stdout,"#include \"%s_reflection.hpp\"\n\n",className.c_str());
    fprintf(stdout,"namespace reflcpp {\n\n");

    // Inserter
    fprintf(stdout,"namespace {\n");
    fprintf(stdout,"    ClassType_tmpl<%s> inserter;\n", className.c_str());
    fprintf(stdout,"}\n\n");


    fprintf(stdout,"const char %s_strings::name[] = \"%s\";\n", className.c_str(),className.c_str());

    for (vector<Xct_Field*>::iterator it = (c->fields).begin();
         it != (c->fields).end(); ++it) {
        fprintf(stdout,"const char %s_strings::f_%s[] = \"%s\";\n", className.c_str(),(*it)->name.c_str(),(*it)->name.c_str());
    }

    for  (vector<Xct_Method*>::iterator it = c->methods.begin();
          it != c->methods.end(); ++it) {
        fprintf(stdout,"const char %s_strings::m_%s[] = \"%s\";\n", className.c_str(),(*it)->name.c_str(),(*it)->name.c_str());
    }

    fprintf(stdout,"} // namespace reflcpp\n");
}


/**
 * given the id of the class
 * generate the cpp file for the class
 */
static void genCpp(char* cid)
{
    xmlNode *n;
    char *_tagName, *className, *_members,*_fid,*_fname;

    n = searchNodeById(cid);
    if (n == NULL) {
        cerr << "Can't find the node with id " << cid << endl;
        return;
    }
    
    if (strcmp(getTag(n),"Class") != 0) {
        cerr << "non-Class definiation found" << endl
             << "You may need to run gccxml with -fxml-start option" << endl;
        return;
    }

    Xct_Class* c = buildClass(n);
    genCpp(c);

    delete c;
}


// --------------------------------------------------------------------------
// generate  c++ code from the DOM
// --------------------------------------------------------------------------
static void genCode(char *nid)
{
//     char *_members, *_base,*_name,*_id,*_tagName,*_retId,*_typeId,*_access,*_virtual;
//     xmlNode *n,*child;

//     n = searchNodeById(nid);
//     if (n == NULL) return;

//     _tagName = XMLString::transcode(n->getTagName());

//     if (strcmp(_tagName, "Class") == 0) {
//         _name = XMLString::transcode(n->getAttribute(XMLString::transcode("name")));
//         printf("Class %s {\n",_name);
    
//         _members = XMLString::transcode(n->getAttribute(XMLString::transcode("members")));
//         _id = strtok(_members, " ");
//         while (_id != NULL) {
//             genCode(_id);
//             _id = strtok(NULL, " ");
//         }
//         printf("}\n");
//     }

//     if (strcmp(_tagName, "Field") == 0) {
//         _name = XMLString::transcode(n->getAttribute(XMLString::transcode("name")));
//         _typeId = XMLString::transcode(n->getAttribute(XMLString::transcode("type")));
//         _access = XMLString::transcode(n->getAttribute(XMLString::transcode("access")));
//         printf("%s ",_access);
//         genCode(_typeId);
//         printf(" : %s\n", _name);
//     }

//     if (strcmp(_tagName, "FundamentalType") == 0) {
//         _name = XMLString::transcode(n->getAttribute(XMLString::transcode("name")));
//         printf("%s ",_name);
//     }

//     if ((strcmp(_tagName, "Method") == 0) || (strcmp(_tagName, "Constructor") == 0) || (strcmp(_tagName, "Destructor") == 0) ) {
//         _name = XMLString::transcode(n->getAttribute(XMLString::transcode("name")));
//         _access = XMLString::transcode(n->getAttribute(XMLString::transcode("access")));
//         _retId = XMLString::transcode(n->getAttribute(XMLString::transcode("return")));
//         _virtual = XMLString::transcode(n->getAttribute(XMLString::transcode("virtual")));
    
//         printf("%s ",_access);
//         if (strcmp(_virtual,"1") == 0 )
//             printf(" virtual ");

//         genCode(_retId);
//         if (strcmp(_tagName, "Destructor") == 0)
//             printf(" ~%s(",_name);
//         else
//             printf(" %s(",_name);

//         for (child = (DOMElement*) n->getFirstChild(); child != 0; child= (DOMElement *) child->getNextSibling()) {
//             _tagName = XMLString::transcode(child->getTagName());
//             if (strcmp(_tagName, "Argument") == 0) {
//                 _name = XMLString::transcode(child->getAttribute(XMLString::transcode("name")));
//                 _typeId = XMLString::transcode(child->getAttribute(XMLString::transcode("type")));
//                 genCode(_typeId);
//                 printf(" %s,",_name);
//             }
//         }
//         printf(")\n ");
//     }

}

// ---------------------------------------------------------------------------
//
//   main
//
// ---------------------------------------------------------------------------
int main(int argC, char* argV[])
{
    // Check command line and extract arguments.
    if (argC < 2)
    {
        usage();
        return 1;
    }

    LIBXML_TEST_VERSION
    
        int fileType = HEAD;
    int argInd;
    for (argInd = 1; argInd < argC; argInd++)
    {
        // Break out on first parm not starting with a dash
        if (argV[argInd][0] != '-')
            break;

        // Watch for special case help request
        if (!strncmp(argV[argInd], "-h",2))
        {
            usage();
            return 2;
        }
        // generation type
        else if (!strncmp(argV[argInd], "-t=", 3)
                 ||  !strncmp(argV[argInd], "-t=", 3))
        {
            const char* const parm = &argV[argInd][3];

            if (!strcmp(parm, "hpp"))
                fileType = HEAD;
            else if (!strcmp(parm, "cpp"))
                fileType = CPP;
            else
            {
                cerr << "Unknown -t= value: " << parm << endl;
                return 2;
            }
        }
        else
        {
            cerr << "Unknown option '" << argV[argInd]
                 << "', ignoring it\n" << endl;
        }
    }

    //
    //  There should be only one and only one parameter left, and that
    //  should be the file name.
    //
    if (argInd != argC - 1)
    {
        usage();
        return 1;
    }


    xmlDocPtr doc = xmlParseFile(argV[argInd]);
    
    if (doc) {
        gRootNode = (xmlNode *) xmlDocGetRootElement(doc);
        if (fileType == HEAD)
            genHpp("_1");
        else
            genCpp("_1");
    }
    else {
        cerr << "bad xml file" << endl;
    }
    

    xmlCleanupParser();
    xmlFreeDoc(doc);
    return 0;
}


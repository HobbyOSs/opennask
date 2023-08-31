#ifndef X86_HH
#define X86_HH

#include <string>
#include <cstdint>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <optional>
#include <jsoncons/json.hpp>
#include "nask_parse_common.hh"
#include "para_token.hh"

namespace x86_64 {

    extern const char* X86_64_JSON;

    const size_t _calc_offset_byte_size(const std::vector<TParaToken>& tokens);
    const bool _require_sib_byte(const std::vector<TParaToken>& tokens);

    const std::string token_to_x86_type(const TParaToken&);
    const bool _starts_with(std::string const &full_string, std::string const &begining);
    const std::string _to_lower(const std::string &in);

    //
    // ここから下はjson-x86-64の読み取り用クラス
    //
    class Isa {
        std::string id_;
    public:
        Isa() {}
        Isa(const std::string& id)
            : id_(id)
        {}

        const std::string id() const
        {
            return id_;
        }
    };

    class DataOffset {
        int size_;
        std::string value_;
    public:
        DataOffset() {}
        DataOffset(const int size,
                   const std::string& value)
            : size_(size),
              value_(value)
        {}

        const int size() const
        {
            return size_;
        }
        const std::string& value() const
        {
            return value_;
        }
    };

    class CodeOffset {
        int size_;
        std::string value_;
    public:
        CodeOffset() {}
        CodeOffset(const int size,
                   const std::string& value)
            : size_(size),
              value_(value)
        {}

        const int size() const
        {
            return size_;
        }
        const std::string& value() const
        {
            return value_;
        }
    };

    class Prefix {
      bool mandatory_;
      std::string byte_;
    public:
        Prefix() {}
        Prefix(const bool mandatory, const std::string& byte)
            : mandatory_(mandatory),
              byte_(byte)
        {}

        const bool mandatory() const
        {
            return mandatory_;
        }
        const std::string& byte() const
        {
            return byte_;
        }
        const int get_size() const
        {
            return 1;  // 0x66 のみの場合がほとんど
        }
    };

    class Rex {
        bool mandatory_;
        std::optional<std::string> W_;
        std::optional<std::string> R_;
        std::optional<std::string> B_;
        std::optional<std::string> X_;
    public:
        Rex() {}
        Rex(const bool mandatory,
            const std::optional<std::string>& W,
            const std::optional<std::string>& R,
            const std::optional<std::string>& B,
            const std::optional<std::string>& X)
            : mandatory_(mandatory),
              W_(W), R_(R), B_(B), X_(X)
        {}

        const bool mandatory() const
        {
            return mandatory_;
        }
        const std::optional<std::string>& W() const
        {
            return W_;
        }
        const std::optional<std::string>& R() const
        {
            return R_;
        }
        const std::optional<std::string>& B() const
        {
            return B_;
        }
        const std::optional<std::string>& X() const
        {
            return X_;
        }
        const int get_size() const
        {
            return 1;  // REXプレフィックスは、オペコードの直前に配置されるため、オペコードサイズを1バイト増やすことになる
        }
    };

    class Vex {
        std::optional<std::string> mmmmm_;
        std::optional<std::string> pp_;
        std::optional<std::string> W_;
        std::optional<std::string> L_;
        std::optional<std::string> R_;
        std::optional<std::string> X_;
    public:
        Vex() {}
        Vex(const std::optional<std::string>& mmmmm,
            const std::optional<std::string>& pp,
            const std::optional<std::string>& W,
            const std::optional<std::string>& L,
            const std::optional<std::string>& R,
            const std::optional<std::string>& X)
            : mmmmm_(mmmmm),
              pp_(pp),
              W_(W), L_(L), R_(R), X_(X)
        {}

        const std::optional<std::string>& mmmmm() const
        {
            return mmmmm_;
        }
        const std::optional<std::string>& pp() const
        {
            return pp_;
        }
        const std::optional<std::string>& W() const
        {
            return W_;
        }
        const std::optional<std::string>& L() const
        {
            // VEXプレフィックスの長さを指定します。Lが0の場合、VEXプレフィックスは3バイトです。Lが1の場合、VEXプレフィックスは2バイト。
            return L_;
        }
        const std::optional<std::string>& R() const
        {
            return R_;
        }
        const std::optional<std::string>& X() const
        {
            return X_;
        }
        const int get_size() const
        {
            int size = 0;
            if (mmmmm_ && pp_) {
                const std::string& mmmmm = mmmmm_.value();
                const std::string& pp = pp_.value();
                int L = L_ ? std::stoi(L_.value(), nullptr, 2) : 1;
                if (mmmmm == "00001" && std::stoi(pp, nullptr, 2) <= 2) {
                    size = (L == 0) ? 3 : 2;
                }
            }
            return size;
        }
    };

    class Modrm {
        std::string mode_;
        std::string rm_;
        std::string reg_;
    public:
        Modrm() {}
        Modrm(const std::string& mode,
              const std::string& rm,
              const std::string& reg)
            : mode_(mode), rm_(rm), reg_(reg)
        {}

        const std::string& mode() const
        {
            return mode_;
        }
        const std::string& rm() const
        {
            return rm_;
        }
        const std::string& reg() const
        {
            return reg_;
        }
        const int get_size() const
        {
            return 1;  // ModRMは常に1byte
        }
    };

    class Operand {
        std::string type_;
        std::optional<bool> input_;
        std::optional<bool> output_;
        std::optional<int> extended_size_;
    public:
        Operand() {}
        Operand(const std::string& type,
                const std::optional<bool> input,
                const std::optional<bool> output,
                const std::optional<int> extended_size)
            : type_(type),
              input_(input),
              output_(output),
              extended_size_(extended_size)
        {}

        const std::string& type() const
        {
            return type_;
        }
        const std::optional<bool> input() const
        {
            return input_;
        }
        const std::optional<bool> output() const
        {
            return output_;
        }
        const std::optional<int> extended_size() const
        {
            return extended_size_;
        }
    };

    class ImplicitOperand {
        std::string id_;
        std::optional<bool> input_;
        std::optional<bool> output_;
    public:
        ImplicitOperand() {}
        ImplicitOperand(const std::string& id,
                        const std::optional<bool> input,
                        const std::optional<bool> output)
            : id_(id),
              input_(input),
              output_(output)
        {}

        const std::string& id() const
        {
            return id_;
        }
        const std::optional<bool> input() const
        {
            return input_;
        }
        const std::optional<bool> output() const
        {
            return output_;
        }
    };

    class Immediate {
        int size_;
        std::string value_;
    public:
        Immediate() {}
        Immediate(const int size,
                  const std::string& value)
            : size_(size),
              value_(value)
        {}

        const int size() const
        {
            return size_;
        }
        const std::string& value() const
        {
            return value_;
        }
    };

    class Opcode {
        std::string byte_;
        std::optional<std::string> addend_;
    public:
        Opcode() {}
        Opcode(const std::string& byte,
               const std::optional<std::string>& addend)
            : byte_(byte),
              addend_(addend)
        {}

        const std::string& byte() const
        {
            return byte_;
        }
        const std::optional<std::string> addend() const
        {
            return addend_;
        }
        const int get_size() const
        {
            return 1;
        }
    };

    // Prefix, REX, VEX, Opcode, ModRM, RegisterByte, Immediate, DataOffset, CodeOffset
    class Encoding {
        Opcode opcode_;
        std::optional<Prefix> prefix_;
        std::optional<Rex> REX_;
        std::optional<Vex> VEX_;
        std::optional<Modrm> ModRM_;
        std::optional<Immediate> immediate_;
        std::optional<DataOffset> data_offset_;
        std::optional<CodeOffset> code_offset_;

    public:
        Encoding() {}
        Encoding(const Opcode& opcode,
                 const std::optional<Prefix>& prefix,
                 const std::optional<Rex>& REX,
                 const std::optional<Vex>& VEX,
                 const std::optional<Modrm>& ModRM,
                 const std::optional<Immediate>& immediate,
                 const std::optional<DataOffset> data_offset,
                 const std::optional<CodeOffset> code_offset)
            : opcode_(opcode),
              prefix_(prefix),
              REX_(REX),
              VEX_(VEX),
              ModRM_(ModRM),
              immediate_(immediate),
              data_offset_(data_offset),
              code_offset_(code_offset)
        {}

        const Opcode& opcode() const
        {
            return opcode_;
        }
        const std::optional<Prefix>& prefix() const
        {
            return prefix_;
        }
        const std::optional<Rex>& REX() const
        {
            return REX_;
        }
        const std::optional<Vex>& VEX() const
        {
            return VEX_;
        }
        const std::optional<Modrm>& ModRM() const
        {
            return ModRM_;
        }
        const std::optional<Immediate>& immediate() const
        {
            return immediate_;
        }
        const std::optional<CodeOffset>& code_offset() const
        {
            return code_offset_;
        }
        const std::optional<DataOffset>& data_offset() const
        {
            return data_offset_;
        }

        const int get_output_size(OPENNASK_MODES mode) const;
    };

    class InstructionForm {
        std::vector<Encoding> encodings_;
        std::optional<std::vector<Operand>> operands_;
        std::optional<std::vector<ImplicitOperand>> implicit_operands_;
        std::optional<std::string> xmm_mode_;
        std::optional<bool> cancelling_inputs_;
        std::optional<std::vector<Isa>> isa_;

    public:
        InstructionForm()
        {}
        InstructionForm(const std::vector<Encoding>& encodings,
                        const std::optional<std::vector<Operand>>& operands,
                        const std::optional<std::vector<ImplicitOperand>>& implicit_operands,
                        const std::optional<std::string>& xmm_mode,
                        const std::optional<bool> cancelling_inputs,
                        const std::optional<std::vector<Isa>>& isa)
            : encodings_(encodings),
              operands_(operands),
              implicit_operands_(implicit_operands),
              xmm_mode_(xmm_mode),
              cancelling_inputs_(cancelling_inputs),
              isa_(isa)
        {}

        const std::vector<Encoding>& encodings() const
        {
            return encodings_;
        }
        const std::optional<std::vector<Operand>>& operands() const
        {
            return operands_;
        }
        const std::optional<std::vector<ImplicitOperand>>& implicit_operands() const
        {
            return implicit_operands_;
        }
        const std::optional<std::string>& xmm_mode() const
        {
            return xmm_mode_;
        }
        const std::optional<bool>& cancelling_inputs() const
        {
            return cancelling_inputs_;
        }
        const std::optional<std::vector<Isa>>& isa() const
        {
            return isa_;
        }

        // 複数機械語の出力があり得る場合がたまにある
        Encoding find_encoding() const;
    };

    class Instruction {
        std::string summary_;
        std::vector<InstructionForm> forms_;

    public:
        Instruction()
        {}

        Instruction(const std::string& summary, const std::vector<InstructionForm>& forms)
            : summary_(summary), forms_(forms)
        {}

        const std::string& summary() const
        {
            return summary_;
        }

        const std::vector<InstructionForm>& forms() const
        {
            return forms_;
        }

        const bool find_greedy(OPENNASK_MODES, const std::vector<TParaToken>&, const InstructionForm&) const;
        const uint32_t get_output_size(OPENNASK_MODES, const std::vector<TParaToken>&) const;
    };

    class InstructionSet {
        std::string instruction_set_;
        std::map<std::string, Instruction> instructions_;

    public:
        InstructionSet(const InstructionSet &p)
            : instruction_set_(p.instruction_set()), instructions_(p.instructions()) {}

        InstructionSet(const std::string &instruction_set,
                     const std::map<std::string, Instruction> &instructions)
          : instruction_set_(instruction_set), instructions_(instructions) {}

      const std::string &instruction_set() const {
          return instruction_set_;
      }

      const std::map<std::string, Instruction> &instructions() const {
          return instructions_;
      }
    };
};

JSONCONS_ALL_CTOR_GETTER_TRAITS(x86_64::Isa, id)
JSONCONS_ALL_CTOR_GETTER_TRAITS(x86_64::Prefix, mandatory, byte)
JSONCONS_N_CTOR_GETTER_TRAITS(x86_64::Rex, 1, mandatory, W, R, B, X)
JSONCONS_N_CTOR_GETTER_TRAITS(x86_64::Vex, 1, mmmmm, pp, W, L, R, X)
JSONCONS_ALL_CTOR_GETTER_TRAITS(x86_64::Modrm, mode, rm, reg)
JSONCONS_ALL_CTOR_GETTER_TRAITS(x86_64::ImplicitOperand, id, input, output)
JSONCONS_N_CTOR_GETTER_TRAITS(x86_64::Operand, 1, type, input, output, extended_size)
JSONCONS_ALL_CTOR_GETTER_TRAITS(x86_64::DataOffset, size, value)
JSONCONS_ALL_CTOR_GETTER_TRAITS(x86_64::CodeOffset, size, value)
JSONCONS_ALL_CTOR_GETTER_TRAITS(x86_64::Immediate, size, value)
JSONCONS_N_CTOR_GETTER_TRAITS(x86_64::Opcode, 1, byte, addend)
JSONCONS_N_CTOR_GETTER_TRAITS(x86_64::Encoding, 1, opcode, prefix, REX, VEX, ModRM, immediate, data_offset, code_offset)
JSONCONS_N_CTOR_GETTER_TRAITS(x86_64::InstructionForm, 1, encodings, operands, implicit_operands, xmm_mode, cancelling_inputs, isa)
JSONCONS_ALL_CTOR_GETTER_TRAITS(x86_64::InstructionSet, instruction_set, instructions)
JSONCONS_ALL_CTOR_GETTER_TRAITS(x86_64::Instruction, summary, forms)

#endif // ! X86_HH

/* Copyright 2011 by Yasuhiro Matsumoto
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <streambuf>
#include <sstream>
#include <ctime>

template <class CharT, class Traits = std::char_traits<CharT> >
class basic_clogger : public std::basic_streambuf<CharT, Traits> {
public:
  typedef std::basic_streambuf<CharT, Traits> streambuf_type;
  typedef Traits traits_t;
  typedef CharT char_t;
  typedef typename traits_t::int_type int_t;
  typedef std::streamsize streamsize_t;
  typedef std::basic_stringstream<char_t, traits_t> sstream_t;

private:
  streambuf_type* sbuf_;
  char_t* cbuf_;
  static int const BUFFER_SIZE = 256;

public:

  basic_clogger(streambuf_type* buf) :sbuf_(buf),
      cbuf_(new char_t[BUFFER_SIZE]) {
    this->setp(cbuf_, cbuf_ + BUFFER_SIZE);
  }

  ~basic_clogger() {
    this->pubsync();
    delete[] cbuf_;
  }

protected:

  int_t
  overflow(int_t c = traits_t::eof()) {
    streamsize_t n = static_cast<streamsize_t>(this->pptr() - this->pbase());
    time_t ct;
    int year, month, day, hour, minute, second;
    time(&ct);
    struct tm* d = localtime(&ct);
    std::basic_stringstream<char_t, traits_t> ss;

    for (streamsize_t i = 0; i < n; i++)
      if (cbuf_[i] == '\n') { n = i; break; }
    if (n) {
#define CLOGGER_PAD4(o,d) if(d<10)o<<'0'<<'0'<<d; else if (d<100)o<<'0'<<d; else o<<d;
#define CLOGGER_PAD2(o,d) if(d<10)o<<'0'<<d; else o<<d;
      CLOGGER_PAD4(ss, d->tm_year + 1900);
      ss << '/';
      CLOGGER_PAD2(ss, d->tm_mon + 1)
      ss << '/';
      CLOGGER_PAD2(ss, d->tm_mday + 1)
      ss << ' ';
      CLOGGER_PAD2(ss, d->tm_hour)
      ss << ':';
      CLOGGER_PAD2(ss, d->tm_min)
      ss << ':';
      CLOGGER_PAD2(ss, d->tm_sec)
#undef CLOGGER_PAD4
#undef CLOGGER_PAD2
      ss << '\t';
      ss << std::basic_string<char_t>(cbuf_, n) << std::endl;
      std::basic_string<char_t> s = ss.str();
      if (sbuf_->sputn(s.c_str(), s.size()) != s.size())
        return traits_t::eof();
      this->setp(cbuf_, cbuf_ + BUFFER_SIZE);
    }

    if (!traits_t::eq_int_type(c, traits_t::eof())) {
      traits_t::assign(*this->pptr(), traits_t::to_char_type(c));
      this->pbump(1);
    }
    return traits_t::not_eof(c);
  }

  int
  sync() {
    if (traits_t::eq_int_type(overflow(traits_t::eof()), traits_t::eof())) {
      return -1;
    }
    return sbuf_->pubsync() == -1 ? -1 : 0;
  }
};

typedef basic_clogger<char> clogger;

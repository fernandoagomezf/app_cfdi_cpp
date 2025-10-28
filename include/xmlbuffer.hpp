#ifndef _XMLCHARPROVIDER_HPP_
#define _XMLCHARPROVIDER_HPP_

#include <string>

namespace cfdi {
    using std::string;
    using std::string_view;
    using std::size_t;

    class XmlBuffer {
        public:
            explicit XmlBuffer(string_view);
            XmlBuffer(string_view content, size_t position);
            
            char peek();
            char read();
            void consume();
            void consume(size_t count);
            bool canRead() const;
            void skipWhiteSpace();

            size_t position() const;
            size_t length() const;
            string substr(size_t pos, size_t len) const;
            string substr(size_t len) const;
            
            static bool isWhiteSpace(char c);

        private:
            string _buffer;
            size_t _position;
    };
}

#endif 

#ifndef __CLI_JINJA__
#define __CLI_JINJA__


#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include "stringhelper.h"

#define VIRTUAL virtual
#define STATIC static

namespace Jinja {

class render_error : public std::runtime_error {
public:
    render_error( const std::string &what ) :
        std::runtime_error( what ) {
    }
};

class Value {
public:
    virtual ~Value() {}
    virtual std::string render() = 0;
    virtual bool isTrue() const = 0;
};
class IntValue : public Value {
public:
    int value;
    IntValue( int value ) :
        value( value ) {
    }
    virtual std::string render() {
        return toString( value );
    }
    bool isTrue() const {
        return value != 0;
    }
};
class FloatValue : public Value {
public:
    float value;
    FloatValue( float value ) :
        value( value ) {
    }
    virtual std::string render() {
        return toString( value );
    }
    bool isTrue() const {
        return value != 0.0;
    }
};
class StringValue : public Value {
public:
    std::string value;
    StringValue( std::string value ) :
        value( value ) {
    }
    virtual std::string render() {
        return value;
    }
    bool isTrue() const {
        return !value.empty();
    }
};

class Root;
class ControlSection;

class Template {
public:
    std::string sourceCode;

    std::map< std::string, Value * > valueByName;
    Root *root;
    Template( std::string sourceCode );
    STATIC bool isNumber( std::string astring, int *p_value );
    VIRTUAL ~Template();
    Template &setValue( std::string name, int value );
    Template &setValue( std::string name, float value );
    Template &setValue( std::string name, std::string value );
    std::string render();
    void print(ControlSection *section);
    int eatSection( int pos, ControlSection *controlSection );
    STATIC std::string doSubstitutions( std::string sourceCode, std::map< std::string, Value *> valueByName );
};

class ControlSection {
public:
    std::vector< ControlSection * >sections;
    virtual std::string render( std::map< std::string, Value *> &valueByName ) = 0;
    virtual void print() {
        print("");
    }
    virtual void print(std::string prefix) = 0;
};

class Container : public ControlSection {
public:
    int sourceCodePosStart;
    int sourceCodePosEnd;

    virtual void print( std::string prefix ) {
        std::cout << prefix << "Container ( " << sourceCodePosStart << ", " << sourceCodePosEnd << " ) {" << std::endl;
        for( int i = 0; i < (int)sections.size(); i++ ) {
            sections[i]->print( prefix + "    " );
        }
        std::cout << prefix << "}" << std::endl;
    }
};

class ForSection : public ControlSection {
public:
    int loopStart;
    int loopEnd;
    std::string varName;
    int startPos;
    int endPos;
    std::string render( std::map< std::string, Value *> &valueByName ) {
        std::string result = "";
        if( valueByName.find( varName ) != valueByName.end() ) {
            throw render_error("variable " + varName + " already exists in this context" );
        }
        for( int i = loopStart; i < loopEnd; i++ ) {
            valueByName[varName] = new IntValue( i );
            for( size_t j = 0; j < sections.size(); j++ ) {
                result += sections[j]->render( valueByName );
            }
            delete valueByName[varName];
            valueByName.erase( varName );
        }
        return result;
    }
    virtual void print( std::string prefix ) {
        std::cout << prefix << "For ( " << varName << " in range(" << loopStart << ", " << loopEnd << " ) {" << std::endl;
        for( int i = 0; i < (int)sections.size(); i++ ) {
            sections[i]->print( prefix + "    " );
        }
        std::cout << prefix << "}" << std::endl;
    }
};

class Code : public ControlSection {
public:
    int startPos;
    int endPos;
    std::string templateCode;

    std::string render();
    virtual void print( std::string prefix ) {
        std::cout << prefix << "Code ( " << startPos << ", " << endPos << " ) {" << std::endl;
        for( int i = 0; i < (int)sections.size(); i++ ) {
            sections[i]->print( prefix + "    " );
        }
        std::cout << prefix << "}" << std::endl;
    }

    virtual std::string render( std::map< std::string, Value *> &valueByName ) {
        std::string processed = Template::doSubstitutions( templateCode, valueByName );
        return processed;
    }
};

class Root : public ControlSection {
public:
    virtual ~Root() {}
    virtual std::string render( std::map< std::string, Value *> &valueByName ) {
        std::string resultString = "";
        for( int i = 0; i < (int)sections.size(); i++ ) {
            resultString += sections[i]->render( valueByName );
        }     
        return resultString;   
    }
    virtual void print(std::string prefix) {
        std::cout << prefix << "Root {" << std::endl;
        for( int i = 0; i < (int)sections.size(); i++ ) {
            sections[i]->print( prefix + "    " );
        }
        std::cout << prefix << "}" << std::endl;
    }    
};

class IfSection : public ControlSection {
public:
    IfSection(const std::string& expression) {
        parseIfCondition(expression);
    }

    std::string render(std::map< std::string, Value *> &valueByName) {
        std::stringstream ss;
        const bool expressionValue = computeExpression(valueByName);
        if (expressionValue) {
            for (size_t j = 0; j < sections.size(); j++) {
                ss << sections[j]->render(valueByName);
            }
        }
        const std::string renderResult = ss.str();
        return renderResult;
    }

    void print(std::string prefix) {
        std::cout << prefix << "if ( " 
            << ((m_isNegation) ? "not " : "") 
            << m_variableName << " ) {" << std::endl;
        if (true) {
            for (int i = 0; i < (int)sections.size(); i++) {
                sections[i]->print(prefix + "    ");
            }
        }
        std::cout << prefix << "}" << std::endl;
    }

private:
    void parseIfCondition(const std::string& expression);
    bool computeExpression(const std::map< std::string, Value *> &valueByName) const;
    bool m_isNegation;
    std::string m_variableName;
};

}

#endif

#include <iostream>
#include "antlr4-runtime.h"
#include "Python3Lexer.h"
#include "Python3Parser.h"
#include "Evalvisitor.h"
#include "bigint.h"
#include "Rec.h"

using namespace antlr4;
//todo: regenerating files in directory named "generated" is dangerous.
//       if you really need to regenerate,please ask TA for help.
int main(int argc, const char* argv[]){
    //todo:please don't modify the code below the construction of ifs if you want to use visitor mode
    /*argv[1]="/home/battlin/CLionProjects/Python_Assignment-master/testData/test4.in";
    const std::string filepath=argv[1];
    //const std::string filepath="../testData/test1.in";
    std::ifstream ifs;
    ifs.open(filepath);
    if (!ifs.good()) {
        std::cout<<"bad";
    }
    ANTLRInputStream input(ifs);
     */
    ANTLRInputStream input(std::cin); //new line
    Python3Lexer lexer(&input);
    CommonTokenStream tokens(&lexer);
    tokens.fill();
    Python3Parser parser(&tokens);
    tree::ParseTree* tree=parser.file_input();
    EvalVisitor visitor;
    visitor.visit(tree);
    //ifs.close();
    return 0;;
}
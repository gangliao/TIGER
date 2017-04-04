#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <map>
#include <list>
#include <stack>
#include <algorithm>
#include <ctype.h>
class asmCodeGenerator{

public:
    std::vector<std::string> ir_code;
    std::vector<std::string> asm_code;
    std::vector<std::string> prog_var;
    std::vector<std::string> prog_func;
    int str_var_num = -1;
    int float_reg_num = -1;
     //constructor, reads the input IR code in buffer
    asmCodeGenerator(std::string filename){

        std::ifstream file;
        file.open(filename);
        std::string line; 
        if(file.is_open()){

            while(std::getline(file, line)){
                ir_code.push_back(line);
            }
        }
        file.close();
    }

    bool present_in_var_list(std::string in_str){
        for (int i =0;i<prog_var.size();i++){
            if(strcmp(prog_var[i].c_str(),in_str.c_str())==0){
                return true;
            }
        }
        return false;
    }
    bool present_in_func_list(std::string in_str){
        for (int i =0;i<prog_func.size();i++){
            if(strcmp(prog_func[i].c_str(),in_str.c_str())==0){
                return true;
            }
        }
        return false;
    }
    std::string gen_var_name(){
        str_var_num++;
        return ("str_" + std::to_string(str_var_num));
    }
    std::string generate_float_reg(){
        float_reg_num++;
        return ("$f" + std::to_string(float_reg_num));
    }
    void generate_asm_code(){

        /****************** data segment ************************/
        std::string bstr =  " ";
        std::string str = ".data";
        asm_code.push_back(str);
        

        for (size_t line_num=0;line_num<ir_code.size();line_num++){

            
            std::string line = ir_code[line_num];
            //get the strings in a buffer
            std::vector<std::string> result;
            std::istringstream iss(line);
            for(std::string line; iss >> line; ){
                result.push_back(line);
            }

            std::string str;
            //name: directive(.word) initializer
            if(strcmp(result[0].c_str(),"load_var") == 0){
                if(present_in_var_list(result[2]) == false){

                    int type = checkType(result[2].c_str());
                    if(type == 1){
                      // std::string str1 = gen_var_name();
                     //   str =  str1 +  ": " + ".word" + " "  + result[2];
                    }
                    else if(type == 2){
                      //  std::string str1 = gen_var_name();
                      //  str =  str1 +  ": " + ".float" + " "  + result[2];
                     }
                    else{
                        str = result[2] + ": " + ".word 0";
                    }
                    asm_code.push_back(str);
                    prog_var.push_back(result[2]);
                }
            }
            else if(strcmp(result[0].c_str(),"store_var") == 0){
                if(present_in_var_list(result[1]) == false){
                    int type = checkType(result[2].c_str());
                    if(type == 1){
                       //str = result[1] + ": " + ".word 0";
                    }
                    else if(type == 2){
                        //str = result[1] + ": " + ".float 0";
                    }
                    else{
                        str = result[1] + ": " + ".word 0";
                    }
                    asm_code.push_back(str);
                    prog_var.push_back(result[1]);
                }
            }
            else if((strcmp(result[0].c_str(),"assign") == 0)&&(result.size()==4)) {
                
               int type1 = checkType(result[3].c_str());
                if((type1 == 1)||(type1 == 2)){
                    int sp = stoi(result[2]) * 4;
                    str = result[1] + ": .space " + std::to_string(sp);
                    asm_code.push_back(str);
                }
            }   
            //store function names
            else if(strcmp(result[0].c_str(),"call") == 0){
                str = result[1]+":";
                prog_func.push_back(str);
            }
            else if(strcmp(result[0].c_str(),"callr") == 0){
                prog_func.push_back(result[2]);
            }
        }
        int codeStartLine = 0;
        //find the code line where variable declaration ends
        for (size_t line_num=0;line_num<ir_code.size();line_num++){
            std::string line = ir_code[line_num];
            //get the strings in a buffer
            std::vector<std::string> result;
            std::istringstream iss(line);
            for(std::string line; iss >> line; ){
                result.push_back(line);
            }
            if((present_in_func_list(result[0])) || (strcmp(result[0].c_str(),"main") == 0)){

                   codeStartLine = line_num;
                   break;
            }
        }
        str = ".text";
        asm_code.push_back(str);
        str = "main:";
        asm_code.push_back(str);
        if(codeStartLine >0){
            parse_ir_code(0, codeStartLine-1);
        }
        str = "j main0";
        asm_code.push_back(str);
        str = "\nli $v0, 10";
        asm_code.push_back(str);
        str = "syscall";
        asm_code.push_back(str);

        //store printi function
        str="printi:";
        asm_code.push_back(str);
        str="li $v0, 1";
        asm_code.push_back(str);
        str="lw $a0, printi_arg0";
        asm_code.push_back(str);
        str="syscall";
        asm_code.push_back(str);
        str="jr $ra";
        asm_code.push_back(str);
        
        parse_ir_code(codeStartLine, ir_code.size()-1);
    }
    void parse_ir_code(int codeStartLine, int endline){

        std::string bstr =  " ";
        std::string str;
        bool loading_float_var = false;
        for (size_t line_num=codeStartLine;line_num<=endline;line_num++) {

            std::string line = ir_code[line_num];
            //get the strings in a buffer
            std::vector<std::string> result;
            std::istringstream iss(line);
            for(std::string line; iss >> line; ){
                result.push_back(line);
            }

            std::string str;
        
            if(strcmp(result[0].c_str(),"main:") == 0) {
                
                str = "main0:";
                asm_code.push_back(str);
            } 
            else if((strcmp(result[0].c_str(),"assign") == 0)&&(result.size()==3)) {
                str = "move " + result[1] + ", " + result[2];
                asm_code.push_back(str);
            } 
            else if((strcmp(result[0].c_str(),"assign") == 0)&&(result.size()==4)) {
                int type = checkType(result[3].c_str());
                double num = 0;
                if(type == 2) {//float
                    num =  stof(result[3]);
                }
                else if(type == 1){ //int
                    num =  stoi(result[3]);
                }
                str = "li.s $f3, " + result[3];
                asm_code.push_back(str);
                str = "la $t0, " + result[1];
                asm_code.push_back(str);
                int size = 4 * stoi(result[2]);
                std::string reg = "($t0)";
                for (int i=4;i<size;i=i+4){
                    str = "swc1 $f3, " + std::to_string(i) + reg;
                    asm_code.push_back(str);
                }
            } 
            else if(strcmp(result[0].c_str(),"return") == 0){
                
                str = "jr $ra";
                asm_code.push_back(str);
            } 
            else if(strcmp(result[0].c_str(),"goto") == 0){
                
                str = "j " + result[1];
                asm_code.push_back(str);
            } 
            else if( (strcmp(result[0].c_str(),"add") == 0) || (strcmp(result[0].c_str(),"sub") == 0) 
                        || (strcmp(result[0].c_str(),"mult") == 0) || (strcmp(result[0].c_str(),"div") == 0)
                        || (strcmp(result[0].c_str(),"and") == 0) || (strcmp(result[0].c_str(),"or") == 0) ){

                int type1 = checkType(result[1].c_str());
                int type2 = checkType(result[2].c_str());
                
                if((type1 == 1) && (type2 == 1)){ //both are integers

                    if((strcmp(result[0].c_str(),"mult") == 0) || (strcmp(result[0].c_str(),"div") == 0))
                    {
                        str = result[0] + bstr + result[1] + ", " + result[2];
                        asm_code.push_back(str);
                        str = "mflo " + result[3];
                        asm_code.push_back(str);
                    }
                    else
                    {
                        str = result[0] + bstr + result[3] + ", " + result[1] + ", " + result[2];
                        asm_code.push_back(str);
                    }
                }
                else if(loading_float_var == true){ //any one of them is float

                        str =result[0] + ".s $f2, $f0, $f1";
                        asm_code.push_back(str);
                        str ="mfc1 " + result[3] + ", $f2";
                        asm_code.push_back(str);

                        /*
                        str ="mtc1 " + result[1] + ", $f0";
                        asm_code.push_back(str);
                        str ="mtc1 " + result[2] + ", $f1";
                        asm_code.push_back(str);

                        str ="cvt.s.w $f1, $f1";
                        asm_code.push_back(str);
                        str =result[0] + ".s $f2, $f0, $f1";
                        asm_code.push_back(str);
                        str ="mfc1 " + result[3] + ", $f2";
                        asm_code.push_back(str);*/
                    }else
                    {
                        str = result[0] + bstr + result[3] + ", " + result[1] + ", " + result[2];
                        asm_code.push_back(str);
                    }
            }
            else if(strcmp(result[0].c_str(),"call") == 0){

              str = "addi $sp, $sp, -8";
              asm_code.push_back(str);
              str = "sw $a0, 4($sp)";
              asm_code.push_back(str);
              str = "sw $ra, 0($sp)";
              asm_code.push_back(str);
              str ="jal " + result[1];
              asm_code.push_back(str);
              str ="lw $a0, 4($sp)";
              asm_code.push_back(str);
              str ="lw $ra, 0($sp)";
              asm_code.push_back(str);
              str ="addi $sp, $sp, 8";
              asm_code.push_back(str);
            }
            else if(strcmp(result[0].c_str(),"load_var") == 0){
                int type = checkType(result[2].c_str());
                if(type == 1){
                  str = "li " + result[1] + ", " + result[2];
                  asm_code.push_back(str);
                  loading_float_var = false;
                }
                else if(type == 2){
                  std::string float_reg = generate_float_reg();
                  str = "li.s "+ float_reg +", "+ result[2];
                  asm_code.push_back(str);
                  str ="mfc1 " + result[1] + " " + float_reg;
                  asm_code.push_back(str);
                  loading_float_var = true;
                }
                else{
                    
                    str = "lw " + result[1] + ", " + result[2];
                    asm_code.push_back(str);
                }
                
            } 
            else if(strcmp(result[0].c_str(),"store_var") == 0){
                int type = checkType(result[1].c_str());
                if((type != 1)&&(type != 2)){
                    
                   str = "sw " + result[2] + ", " + result[1];
                    asm_code.push_back(str);
                }
            } 
            else if(strcmp(result[0].c_str(),"array_store") == 0){
                int type = checkType(result[2].c_str());
                if(type == 1) //integer
                {
                    int offset = stoi(result[2]);
                    offset *= 4;
                    str = "sw " + result[3] + ", " + result[1] + "(" + std::to_string(offset) + ")";
                    asm_code.push_back(str);
                }
                else
                {
                    str = "li $t8, 4";
                    asm_code.push_back(str);
                    str="mult $t8, " + result[2];
                    asm_code.push_back(str);
                    str = "mflo $t8";
                    asm_code.push_back(str);
                    str="sw " + result[3] + ", " + result[1] + "($t8)";
                    asm_code.push_back(str);
                }
            }
            else if(strcmp(result[0].c_str(),"array_load") == 0){
                int type = checkType(result[3].c_str());
                if(type == 1) //integer
                {
                    int offset = stoi(result[3]);
                    offset *= 4;
                    str = "lw " + result[1] + ", " + result[2] + "(" + std::to_string(offset) + ")";
                    asm_code.push_back(str);
                }
                else
                {
                    str = "li $t8, 4";
                    asm_code.push_back(str);
                    str="mult $t8, " + result[3];
                    asm_code.push_back(str);
                    str = "mflo $t8";
                    asm_code.push_back(str);
                    str="lw " + result[1] + ", " + result[2] + "($t8)";
                    asm_code.push_back(str);
                }
            }
            else if(strcmp(result[0].c_str(),"breq") == 0){

                str = "beq " + result[1] + ", " + result[2] + ", " + result[3];
                asm_code.push_back(str);

            }
            else if(strcmp(result[0].c_str(),"brneq") == 0){
                str = "bne " + result[1] + ", " + result[2] + ", " + result[3];
                asm_code.push_back(str);
            }
            else if(strcmp(result[0].c_str(),"brlt") == 0){
                str = "blt " + result[1] + ", " + result[2] + ", " + result[3];
                asm_code.push_back(str);
            }
            else if(strcmp(result[0].c_str(),"brgt") == 0){
                str = "bgt " + result[1] + ", " + result[2] + ", " + result[3];
                asm_code.push_back(str);
            }
            else if(strcmp(result[0].c_str(),"brgeq") == 0){
                str = "bge " + result[1] + ", " + result[2] + ", " + result[3];
                asm_code.push_back(str);
            }
            else if(strcmp(result[0].c_str(),"brleq") == 0){
                str = "ble " + result[1] + ", " + result[2] + ", " + result[3];
                asm_code.push_back(str);
            }
            else{
                
                if(present_in_func_list(result[0])){

                    asm_code.push_back(result[0]);
                    std::string float_reg = generate_float_reg();
                    //std::string str1 = "lwc1 $f0, ";          
                    std::string str1 = "lwc1 " + float_reg +", ";
                    str = str1 + "4($sp)";
                    asm_code.push_back(str);
                    //no need to add any space in stack
                    //str ="addi $sp, $sp, 8";
                    //asm_code.push_back(str);
                }
                else{
                    int isdone = 0;
                    for (size_t char_num =0;char_num<result[0].size();char_num++){
                            if(result[0][char_num] == ':'){
                                asm_code.push_back(result[0]);
                                isdone = 1;
                                break;
                            }
                        }
                    if(isdone==0)
                        std::cout << "I am here **********" << line <<std::endl;
                }
            }
        }
    }
    int checkType(const char *input)
    {
        short int isInt=0;//we use int as a boolean value;
        short int isFloat=0;
        short int isUndifined=0;
        int count;
        int len = strlen(input);
        for(count = 0 ; count<len; count++)
        {
            if(isdigit(input[count])){//you should include ctype.h at the beginning of this program
                isInt=1;
                //std::cout << "got int ";
            }
            else if(input[count] == '.'){
                isFloat=1;
                //std::cout << "got float ";
            }
            else{
                //std::cout << "dnt know ";
                return -1;//some character that neither int nor '.' char.
            }
        }
        if(isInt == 1 && isFloat ==1){
            //std::cout << "Returning float ";
            return 2; // for float
        }
        else if(isInt == 1 && isFloat ==0){
            //std::cout << "Returning int ";
            return 1; //for int
        }
        else{
            
            //std::cout << "Returning unknown";
            return -1;//illegal format
        }
    }

    void print_ircode(){

        for(int i=0;i<ir_code.size();i++){

            std::cout << ir_code[i] <<std::endl;
        }
        std::cout << "************Assembly code****************" <<std::endl;
        for(int i=0;i<asm_code.size();i++){

            std::cout << asm_code[i] <<std::endl;
        }
    }
};


//Pass IR file as input
int main(int argc, char** argv) {

  // The user has given us a bad number of args
  if (argc > 3 || argc < 1) {
    return 0;
  }

  asmCodeGenerator asm_code_gen(argv[1]);
  
  asm_code_gen.generate_asm_code();
  asm_code_gen.print_ircode();
  return 0;
}

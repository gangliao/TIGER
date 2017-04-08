
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>

class naiveAllocationScheme{

public:
    std::vector<std::string> IR;
    std::vector<std::string> naiveRegAllocBuffer;
    int numTemps = 0;
     //constructor, reads the input IR code in buffer
     naiveAllocationScheme(std::string filename){

        std::ifstream file;
        file.open(filename);
        std::string line; 
        if(file.is_open()){

            while(std::getline(file, line)){
                IR.push_back(line);
            }
        }
        file.close();
    }

    std::string new_temp() { return "$t" + std::to_string(numTemps++); }

    void doNaiveRegisterAllocation(){

        for (size_t i = 0; i < IR.size(); ++i) {
            //std::cout << IR[i] << std::endl;

            //remove comma from the line
            std::string s = IR[i];
            for (size_t j =0;j<s.size();j++){

                if(s[j] == ','){
                    s[j] = ' ';
                }
            }

            //get the strings in a line
            std::vector<std::string> result;
            std::istringstream iss(s);
            for(std::string s; iss >> s; ){
                result.push_back(s);
            }

            std::string space = "    ";
            //check for first string in the line
            if((strcmp(result[0].c_str(),"assign") == 0) && (result.size()==3)){
                
                std::string temp1 = new_temp();
                std::string str = space + "load " + temp1 + " " + result[2];
                naiveRegAllocBuffer.push_back(str);

                std::string temp2 = new_temp();
                str = space + "assign " + temp2 + " " + temp1;
                naiveRegAllocBuffer.push_back(str);

                str = space +"store " + result[1] + " " + temp2;
                naiveRegAllocBuffer.push_back(str);
            }
            else if( (strcmp(result[0].c_str(),"brneq") == 0) || (strcmp(result[0].c_str(),"breq") == 0) 
                || (strcmp(result[0].c_str(),"brlt") == 0) || (strcmp(result[0].c_str(),"brgt") == 0)
                || (strcmp(result[0].c_str(),"brgeq") == 0) || (strcmp(result[0].c_str(),"brleq") == 0) ){

                std::string temp1 = new_temp();
                std::string str = space + "load" + " " + temp1 + " " + result[1];
                naiveRegAllocBuffer.push_back(str);

                std::string temp2 = new_temp();
                str = space + "load" + " " + temp2 + " " + result[2];
                naiveRegAllocBuffer.push_back(str);

                str = space + result[0] + " " + temp1 + " " + temp2 + " " + result[3];
                naiveRegAllocBuffer.push_back(str);

            }
            else if( (strcmp(result[0].c_str(),"add") == 0) || (strcmp(result[0].c_str(),"sub") == 0) 
                || (strcmp(result[0].c_str(),"mult") == 0) || (strcmp(result[0].c_str(),"div") == 0)
                || (strcmp(result[0].c_str(),"and") == 0) || (strcmp(result[0].c_str(),"or") == 0) ){

                    std::string temp1 = new_temp();
                    std::string str = space + "load" + " " + temp1 + " " + result[1];
                    naiveRegAllocBuffer.push_back(str);

                    std::string temp2 = new_temp();
                    str = space + "load" + " " + temp2 + " " + result[2];
                    naiveRegAllocBuffer.push_back(str);

                    std::string temp3 = new_temp();
                    str = space + result[0] + " " + temp1 + " " + temp2 + " " + temp3;
                    naiveRegAllocBuffer.push_back(str);

                    str = space + result[0] + "store " + result[3] + " " + temp3;
                    naiveRegAllocBuffer.push_back(str);
            }
            else{

                naiveRegAllocBuffer.push_back(IR[i]);
            }
      }
        
    }

    void print_naive_regalloc_code() {
      std::cout << "\n\n----------------------------------------" << std::endl;
      std::cout << "Generate Reg alloc CODE ..." << std::endl;
      std::cout << "----------------------------------------" << std::endl;
      for (size_t i = 0; i < naiveRegAllocBuffer.size(); ++i) {
        std::cout << naiveRegAllocBuffer[i] << std::endl;
      }
      std::cout << "----------------------------------------\n" << std::endl;
    }
};


//Pass IR file as input
int main(int argc, char** argv) {
  // The user has given us a bad number of args
  if (argc > 3 || argc < 1) {
    return 0;
  }
   
 //do naive register allocation
  naiveAllocationScheme naiveAllocator(argv[1]);
  naiveAllocator.doNaiveRegisterAllocation();
  naiveAllocator.print_naive_regalloc_code();

  return 0;
}

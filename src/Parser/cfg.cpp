

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <map>
class cfgNode{

    public:
      int start;
      int end;
      int blockNum;
      std::vector<cfgNode> edges;
      typedef std::pair<std::string, int> PAIR;
      std::vector<PAIR> def_set;
      std::vector<PAIR> use_set;

      cfgNode(int bNum, int s){

        blockNum = bNum;
        start=s;
        
      }

      void setEndLine(int e){

        end = e;
      }
      void addEdge(cfgNode node){

        edges.push_back(node);
      }

      void printEdges(){

        for(size_t i=0;i<edges.size();i++)
            std::cout << "Edge: From->" << this->blockNum << " to->" << edges[i].blockNum << std::endl;
      }

      void set_def_set(std::string str,int lineNum){

        this->def_set.push_back(std::pair<std::string, int>(str,lineNum));

      }
      void set_use_set(std::string str,int lineNum){

        this->use_set.push_back(std::pair<std::string, int>(str,lineNum));

      }

      void print_du(){
        std::cout << "def=> " ;
        for (auto it=def_set.begin(); it!=def_set.end(); ++it)
                std::cout << it->first << "(" << it->second  << ")";
        std::cout << std::endl;
        std::cout << "use=> " ;
        for (auto it=use_set.begin(); it!=use_set.end(); ++it)
                std::cout << it->first << "(" << it->second << ")";
        std::cout << std::endl;
        
      }

};

class cfg {

    public:
         std::vector<std::string> ir_code;
         std::vector<cfgNode> graph;
         bool next_line_is_new_block;
         int block_number=0;
         //data structure for live range
         struct live_range_blk_line{
                int start_block;
                int end_block;
                int start_line;
                int end_line;
            }details;
          typedef std::map<std::string, live_range_blk_line > Maptype;
          Maptype  live_range;

         //constructor, reads the input IR code in buffer
         cfg(std::string filename){

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

        //generates the block number
        int getBlockNumber(){

            ++block_number;
            return block_number;
        }

        //create basic blocks
        void createBasicBlocks(){

            
            //for first line of code
            next_line_is_new_block = true;

            for (size_t line_num=0;line_num<ir_code.size();line_num++){
          
                int startLine = line_num;

                std::cout << ir_code[line_num] << std::endl ;
                std::string line = ir_code[line_num];

                //remove comma
                for (size_t char_num =0;char_num<line.size();char_num++){

                    if(line[char_num] == ','){
                        line[char_num] = ' ';
                    }
                }

                //get the strings in a line
                std::vector<std::string> line_buffer;
                std::istringstream iss(line);
                for(std::string line; iss >> line; ){
                    line_buffer.push_back(line);
                }

                //first line of the code
                if(next_line_is_new_block){

                    auto node = new cfgNode(getBlockNumber(),startLine);
                    graph.push_back(*node);
                    next_line_is_new_block = false;
                }
                else{

                    std::size_t found = line.find(':');
                    if (found != std::string::npos){
                        //std::cout << "found ':' at: " << found << '\n';
                        auto node = new cfgNode(getBlockNumber(),startLine);
                        graph.push_back(*node);
                    }
                }

                if( (strcmp(line_buffer[0].c_str(),"brneq") == 0) || (strcmp(line_buffer[0].c_str(),"breq") == 0) 
                || (strcmp(line_buffer[0].c_str(),"brlt") == 0) || (strcmp(line_buffer[0].c_str(),"brgt") == 0)
                || (strcmp(line_buffer[0].c_str(),"brgeq") == 0) || (strcmp(line_buffer[0].c_str(),"brleq") == 0) ) {
                    next_line_is_new_block = true;
                    //std::cout << "setting new block to true " << std::endl;
                }   
            }
            std::cout << "Block " << "start line " << "end line " <<std::endl;
            std::cout << "*******************"<<std::endl;
            //set end line for each block
            size_t node_num =0;
            for(node_num = 0;node_num < graph.size() - 1; node_num++){

                    graph[node_num].setEndLine(graph[node_num+1].start - 1);
                    std::cout << "Block: " << graph[node_num].blockNum << " "<<graph[node_num].start << " " << graph[node_num].end << std::endl;
            }

            graph[node_num].setEndLine(ir_code.size()-1);
            std::cout << "Block: " << graph[node_num].blockNum << " " << graph[node_num].start << " " << graph[node_num].end << std::endl;

            //create and print edges
            createEdges(graph, ir_code);
            for(node_num=0;node_num<graph.size();node_num++){
                graph[node_num].printEdges();
            }
            calculate_def_use_set(graph, ir_code);
            for(node_num=0;node_num<graph.size();node_num++){
                graph[node_num].print_du();
            }
            calculate_live_range(graph);
        }
        
        void createEdges(std::vector<cfgNode> &graph, std::vector<std::string> &ir_code) {

            // add edges to the control flow graph
            for(size_t node_num=0;node_num<graph.size();node_num++){

                //get node
                int start_line = graph[node_num].start;
                int end_line = graph[node_num].end;
                std::string label_to_go;

                //get the endline code
                std::string line = ir_code[end_line];

                //remove comma and colon
                for (size_t char_num =0;char_num<line.size();char_num++){
                    if((line[char_num] == ',') || (line[char_num] == ':')){
                        line[char_num] = ' ';
                    }
                }

                //get the strings in a buffer
                std::vector<std::string> line_buffer;
                std::istringstream iss(line);
                for(std::string line; iss >> line; ){
                    line_buffer.push_back(line);
                }

                //if line starts with goto; then next parameter (label) is the block name
                if(strcmp(line_buffer[0].c_str(),"goto") == 0) {

                    //add edge for label
                    label_to_go = line_buffer[1];
                    add_edge(node_num,label_to_go,graph,ir_code);
                    //std::cout << "got goto in block starts with line " << start_line << " " << label_to_go <<std::endl;
                    
                } 
                else if ((strcmp(line_buffer[0].c_str(),"brneq") == 0) || (strcmp(line_buffer[0].c_str(),"breq") == 0) 
                || (strcmp(line_buffer[0].c_str(),"brlt") == 0) || (strcmp(line_buffer[0].c_str(),"brgt") == 0)  
                || (strcmp(line_buffer[0].c_str(),"brgeq") == 0) || (strcmp(line_buffer[0].c_str(),"brleq") == 0) ){

                    // add edge for label
                    label_to_go = line_buffer[3];
                    add_edge(node_num,label_to_go,graph,ir_code);
                    
                    //fall through case
                    if(node_num < graph.size()-1){
                        graph[node_num].addEdge(graph[node_num+1]);
                    }
                }
                else {
                        //fall through case
                        if(node_num < graph.size()-1){
                            graph[node_num].addEdge(graph[node_num+1]);
                        }
                }

            }

        } //end of createEdges

        void add_edge(int src_node_index, std::string label_to_go,std::vector<cfgNode> &graph, std::vector<std::string> &ir_code){

            for(size_t node_num=0;node_num<graph.size();node_num++){

                //get the code of end line
                int start_line = graph[node_num].start;
                std::string line = ir_code[start_line];

                //remove comma
                for (size_t char_num =0;char_num<line.size();char_num++){
                    if((line[char_num] == ',') || (line[char_num] == ':')){
                        line[char_num] = ' ';
                    }
                }

                //get the strings in a line
                std::vector<std::string> line_buffer;
                std::istringstream iss(line);
                for(std::string line; iss >> line; ){
                    line_buffer.push_back(line);
                }
                
                //std::string label_str = label_to_go + ':' ;
                //std::cout << "Comparing strings: " << line_buffer[0] << " " << label_to_go << std::endl;
                if( (strcmp(line_buffer[0].c_str(),label_to_go.c_str()) == 0)){

                    graph[src_node_index].addEdge(graph[node_num]);
                    //std::cout << "creating edge between blocks: " << graph[node_index].start << " " << graph[k].start << std::endl;
                }

            } 
        } //end of add_edge


        void calculate_def_use_set(std::vector<cfgNode> &graph, std::vector<std::string> &ir_code){

            for(size_t node_num=0;node_num<graph.size();node_num++){

                //get the code of end line
                int start_line = graph[node_num].start;
                int end_line = graph[node_num].end;

                for(size_t line_num = start_line;line_num <= end_line; line_num++){
                    std::string line = ir_code[line_num];

                    //std::cout << line << std::endl;
                    //remove comma
                    for (size_t char_num =0;char_num<line.size();char_num++){
                        if((line[char_num] == ',') || (line[char_num] == ':')){
                            line[char_num] = ' ';
                        }
                    }

                    //get the strings in a line
                    std::vector<std::string> result;
                    std::istringstream iss(line);
                    for(std::string line; iss >> line; ){
                        result.push_back(line);
                    }
                    if((strcmp(result[0].c_str(),"assign") == 0) && (result.size()==3)){
                        graph[node_num].set_def_set(result[1],line_num);
                        graph[node_num].set_use_set(result[2],line_num);     

                        //std::cout << result[0] << " " << result[1] << " " <<result[2] <<std::endl;                       
                    }
                    else if((strcmp(result[0].c_str(),"assign") == 0) && (result.size()==4)){
                        graph[node_num].set_def_set(result[1],line_num);
                        graph[node_num].set_use_set(result[2],line_num);     
                        graph[node_num].set_use_set(result[3],line_num);  
                        //std::cout << result[0] << " " << result[1] << " " <<result[2] <<std::endl;                       
                    }
                    else if( (strcmp(result[0].c_str(),"brneq") == 0) || (strcmp(result[0].c_str(),"breq") == 0) 
                        || (strcmp(result[0].c_str(),"brlt") == 0) || (strcmp(result[0].c_str(),"brgt") == 0)
                        || (strcmp(result[0].c_str(),"brgeq") == 0) || (strcmp(result[0].c_str(),"brleq") == 0) ){

                        graph[node_num].set_use_set(result[1],line_num);
                        graph[node_num].set_use_set(result[2],line_num);

                    }
                    else if( (strcmp(result[0].c_str(),"add") == 0) || (strcmp(result[0].c_str(),"sub") == 0) 
                        || (strcmp(result[0].c_str(),"mult") == 0) || (strcmp(result[0].c_str(),"div") == 0)
                        || (strcmp(result[0].c_str(),"and") == 0) || (strcmp(result[0].c_str(),"or") == 0) ){

                        graph[node_num].set_def_set(result[3],line_num);
                        graph[node_num].set_use_set(result[1],line_num);
                        graph[node_num].set_use_set(result[2],line_num);
                    }

                }
            }

        }

        void calculate_live_range(std::vector<cfgNode> &graph){

            int across_block = 0;
            for(size_t node_num=0;node_num<graph.size();node_num++){

                for (auto itd=graph[node_num].def_set.begin(); itd!=graph[node_num].def_set.end(); ++itd){

                    details.start_line = itd->second;
                    details.end_line = itd->second;
                    details.start_block = node_num+1;
                    details.end_block = node_num+1;
                    //this for loop is required if we find live ranges accorss the blocks
                    if(across_block){
                        for(size_t next_node=node_num; next_node<graph.size();next_node++){

                          for (auto itu=graph[next_node].use_set.begin(); itu!=graph[next_node].use_set.end(); ++itu){
                            
                                if(itd->first == itu->first){
                                    details.end_line = itu->second;
                                    details.end_block = next_node+1;
                                    //std::cout << "def for " << itd->first << " use set = " << itu->first << " " << next_node <<std::endl;
                                }
                            }
                        }
                    }
                    else{
                      
                      for (auto itu=graph[node_num].use_set.begin(); itu!=graph[node_num].use_set.end(); ++itu){
                        
                            if(itd->first == itu->first){
                                details.end_line = itu->second;
                                details.end_block = node_num+1;
                            }
                        }
                    }//end of across_block else

                    std::string str = itd->first;
                    live_range.insert(std::pair<std::string, live_range_blk_line > (str,details));
                }

            }
            std::cout << "Live ranges are: " <<std::endl;
            for (auto it=live_range.begin(); it!=live_range.end(); ++it)
                std::cout << it->first << " => " << it->second.start_block << " " << it->second.end_block  << " " << it->second.start_line << " " << it->second.end_line  << '\n';
        }

};

//Pass IR file as input
int main(int argc, char** argv) {

  
  // The user has given us a bad number of args
  if (argc > 3 || argc < 1) {
    return 0;
  }

  cfg graph(argv[1]);
  graph.createBasicBlocks();
  
  return 0;
}

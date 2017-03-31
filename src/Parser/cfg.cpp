#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <map>
#include <list>
#include <stack>
#include <algorithm>

class cfgNode{

    public:
      int start;  //start line of the block
      int end;    // end line of the block
      int blockNum;
      std::vector<cfgNode> edges;
      
      typedef struct def_set_details{
        int usage_count;
        int block_num;
        int line_num;
      }def_use_set;
      
      typedef std::vector<def_use_set>  def_use_vec;
      std::map<std::string, def_use_vec> def_set;
      std::map<std::string, def_use_vec> use_set;

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
            std::cout << "Edge: From block -> " << this->blockNum << " to " << edges[i].blockNum << std::endl;
      }

      //This function sets the def set of the variable
      void set_def_set(std::string str,int blockNum,int lineNum){

        auto it = def_set.find(str);
        //new entry
        if (it == def_set.end()) {
            int count = 1;
            def_use_set var = {count,blockNum,lineNum};
            def_use_vec vec;
            vec.push_back(var);
            this->def_set.insert(std::pair<std::string, def_use_vec>(str,vec));
        }
        else{
            //def entry already there, extend it
            auto str = it->first;
            auto vec = it->second;
            int vec_size = vec.size();
            int count = vec_size + 1;
            def_use_set var = {count,blockNum,lineNum};
            it->second.push_back(var);
            this->def_set.insert(std::pair<std::string, def_use_vec>(str,it->second));
        }
      }
      //This function sets the use set of the variable
      void set_use_set(std::string str,int blockNum,int lineNum){

        auto it = use_set.find(str);
        //new entry
        if (it == use_set.end()) {
            int count = 1;
            def_use_set var = {count,blockNum,lineNum};
            def_use_vec vec;
            vec.push_back(var);
            this->use_set.insert(std::pair<std::string, def_use_vec>(str,vec));
        }
        else{
            //use already present, extend it
            auto str = it->first;
            auto vec = it->second;
            int vec_size = vec.size();
            int count = vec_size + 1;
            def_use_set var = {count,blockNum,lineNum};
            it->second.push_back(var);
            this->use_set.insert(std::pair<std::string, def_use_vec>(str,it->second));
        }
        
      }

      //This function prints the def and use sets of the given block
      void print_du(int blockNum){

        std::cout << "def set for block " << blockNum << " =>";
        for (auto it=def_set.begin(); it!=def_set.end(); ++it){
            
            auto vec = it->second;
            int l = it->second.size();
            for (int j=0;j<l;j++){

                if(blockNum == vec[j].block_num)
                    std::cout << it->first << " (" << vec[j].usage_count << "," << vec[j].block_num << " " << vec[j].line_num << ") ";
            }
        }
        std::cout << std::endl;
        std::cout << "use set for block " << blockNum << " =>";
        for (auto it=use_set.begin(); it!=use_set.end(); ++it){
            
            auto vec = it->second;
            int l = it->second.size();
            for (int j=0;j<l;j++){
                if(blockNum == vec[j].block_num)
                    std::cout << it->first << " (" << vec[j].usage_count << "," << vec[j].block_num << " " << vec[j].line_num << ") ";
            }
        }
        std::cout << std::endl;
        
      }

};
//Class for interference graph nodes, this is used in interblock register allocation
class interferenceGraphNode{

    public:
      int start_line;
      int end_line;
      int start_block;
      int end_block;
      int spill_cost;
      int assigned_color;
      bool isSpillEnable;
      std::string node_name; //name of the node
      std::string var_name;
      std::string assigned_reg;

    std::vector<interferenceGraphNode> edges;
    //vector representing the avilable registers for a node
    std::vector<int> avilable_colors;

    interferenceGraphNode(std::string node_name,std::string var_name,int startline, int endline,int start_block,int end_block){
        
        this->node_name = node_name;
        this->var_name = var_name;
        this->start_line = startline;
        this->end_line = endline;
        this->spill_cost = 0;
        this->start_block = start_block;
        this->end_block = end_block;
        this->assigned_color = -1;
        this->isSpillEnable = false;
        for (int i=0; i< 15;i++){   //number of avilable registers is 15
            avilable_colors.push_back(i);
        }
      }

    void addEdge(interferenceGraphNode &node){
        edges.push_back(node);
      }

    void erase_color(int color){
        
        for (int i=0;i<avilable_colors.size();i++){

            if(avilable_colors[i] == color){
                this->avilable_colors.erase(avilable_colors.begin()+i);
            }
        }
    }

    //generate register name from the assinged color
    void assign_register(int color){

        assigned_reg = "reg" + std::to_string(color);
    }

    void printEdges(){
        for(size_t i=0;i<this->edges.size();i++)
            std::cout << "Edge: From -> " << this->var_name  << "," << this->node_name << " to " << this->edges[i].node_name << std::endl;
        std::cout << "*******************************" <<std::endl;
      }

  };

// This class is for control flow graph
class cfg {

    public:
         std::vector<std::string> ir_code;
         std::vector<std::string> final_ir_code;
         std::vector<cfgNode> graph;
         std::vector<interferenceGraphNode> interferenceGraph;
         std::stack<std::string> interferenceStack;

         bool next_line_is_new_block;
         int block_number=-1;
         int num_registers = 15;
         
         //data structure for live range
         struct live_range_details{
                std::string str_name;
                int start_block;
                int end_block;
                int start_line;
                int end_line;
          };
          typedef std::vector<live_range_details > live_range_vec;
          live_range_vec live_range;

          /************************************************************************/
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
        struct register_entry
        {
            int usage_count;
            int start_block;
            int end_block;
            std::string reg_name;
            std::string string_name;

            register_entry(int k, int sb,int eb,const std::string& r, const std::string& s) : usage_count(k), start_block(sb),end_block(eb),reg_name(r),string_name(s) {}
        };
        std::vector<register_entry> register_map;

        struct sort_on_usage_count
        {
            inline bool operator() (const register_entry& struct1, const register_entry& struct2)
            {
                return (struct1.usage_count > struct2.usage_count);
            }
        };
        struct sort_on_spill_cost
        {
            inline bool operator() (const interferenceGraphNode& struct1, const interferenceGraphNode& struct2)
            {
                return (struct1.spill_cost < struct2.spill_cost);
            }
        };

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
                    
                }   
            }

            //Print the block detials
            std::cout <<"Block details => " <<std::endl;
            std::cout << "\t" <<"Num" << "\t"<< "start" << "\t"<< "end" <<std::endl;
            std::cout << "\t" << "*******************"<<std::endl;
            //set end line for each block
            size_t node_num =0;
            for(node_num = 0;node_num < graph.size() - 1; node_num++){

                    graph[node_num].setEndLine(graph[node_num+1].start - 1);
                    std::cout << "\t" << graph[node_num].blockNum << "\t"<<graph[node_num].start << "\t" << graph[node_num].end << std::endl;
            }

            graph[node_num].setEndLine(ir_code.size()-1);
            std::cout << "\t" << graph[node_num].blockNum << "\t" << graph[node_num].start << "\t" << graph[node_num].end << std::endl;
            std::cout << "\t" << "*******************"<<std::endl;
        }

        //return the block number for the input line number
        int get_block_number(std::vector<cfgNode> &graph,int line_num){

            for(int node_num = 0;node_num < graph.size(); node_num++){
                if((line_num >= graph[node_num].start) && ((line_num <= graph[node_num].end))){
                    //std::cout << "Get block number returning......." << node_num << std::endl;
                    return node_num;
                }
            }
            return -1;
        }

        void create_cfg_edges(std::vector<cfgNode> &graph, std::vector<std::string> &ir_code) {

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
                    add_cfg_edge(node_num,label_to_go,graph,ir_code);
                    //std::cout << "got goto in block starts with line " << start_line << " " << label_to_go <<std::endl;
                    
                } 
                else if ((strcmp(line_buffer[0].c_str(),"brneq") == 0) || (strcmp(line_buffer[0].c_str(),"breq") == 0) 
                || (strcmp(line_buffer[0].c_str(),"brlt") == 0) || (strcmp(line_buffer[0].c_str(),"brgt") == 0)  
                || (strcmp(line_buffer[0].c_str(),"brgeq") == 0) || (strcmp(line_buffer[0].c_str(),"brleq") == 0) ){

                    // add edge for label
                    label_to_go = line_buffer[3];
                    add_cfg_edge(node_num,label_to_go,graph,ir_code);
                    
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

        } //end of create_cfg_edges

        void add_cfg_edge(int src_node_index, std::string label_to_go,std::vector<cfgNode> &graph, std::vector<std::string> &ir_code){

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
        } //end of add_cfg_edge


        void calculate_def_use_set(std::vector<cfgNode> &graph, std::vector<std::string> &ir_code){

            for(size_t node_num=0;node_num<graph.size();node_num++){

                //get the code of end line
                int start_line = graph[node_num].start;
                int end_line = graph[node_num].end;

                for(size_t line_num = start_line;line_num <= end_line; line_num++){
                    std::string line = ir_code[line_num];

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
                        graph[node_num].set_def_set(result[1],node_num,line_num);
                        graph[node_num].set_use_set(result[2],node_num,line_num);     

                        //std::cout << result[0] << " " << result[1] << " " <<result[2] <<std::endl;                       
                    }
                    else if((strcmp(result[0].c_str(),"assign") == 0) && (result.size()==4)){
                        //check if this is valid case ??????????????????
                        //graph[node_num].set_def_set(result[1],node_num,line_num);
                        //graph[node_num].set_use_set(result[2],node_num,line_num);     
                        //graph[node_num].set_use_set(result[3],node_num,line_num);  
                        
                    }
                    else if( (strcmp(result[0].c_str(),"brneq") == 0) || (strcmp(result[0].c_str(),"breq") == 0) 
                        || (strcmp(result[0].c_str(),"brlt") == 0) || (strcmp(result[0].c_str(),"brgt") == 0)
                        || (strcmp(result[0].c_str(),"brgeq") == 0) || (strcmp(result[0].c_str(),"brleq") == 0) ){

                        graph[node_num].set_use_set(result[1],node_num,line_num);
                        graph[node_num].set_use_set(result[2],node_num,line_num);

                    }
                    else if( (strcmp(result[0].c_str(),"add") == 0) || (strcmp(result[0].c_str(),"sub") == 0) 
                        || (strcmp(result[0].c_str(),"mult") == 0) || (strcmp(result[0].c_str(),"div") == 0)
                        || (strcmp(result[0].c_str(),"and") == 0) || (strcmp(result[0].c_str(),"or") == 0) ){

                        graph[node_num].set_def_set(result[3],node_num,line_num);
                        graph[node_num].set_use_set(result[1],node_num,line_num);
                        graph[node_num].set_use_set(result[2],node_num,line_num);
                    }
                    else if((strcmp(result[0].c_str(),"call") == 0) || (strcmp(result[0].c_str(),"callr") == 0)){
                        int offset = 0;

                        if(strcmp(result[0].c_str(),"callr") == 0){
                            offset = 1;
                            graph[node_num].set_def_set(result[1],node_num,line_num);  //return value
                        }

                        for (size_t char_num =2+offset;char_num<result.size();char_num++){
                            graph[node_num].set_use_set(result[char_num],node_num,line_num);     
                            //std::cout << result[0] << " " << result[1] << " " <<result[2] <<std::endl;                       
                        }
                    }
                    else if((strcmp(result[0].c_str(),"return") == 0) && (result.size()==2)){

                        graph[node_num].set_use_set(result[1],node_num,line_num);     
                                             
                    }
                    else if(strcmp(result[0].c_str(),"goto") == 0){
                                              
                    }
                    else if((strcmp(result[0].c_str(),"return") == 0) && (result.size()==1)){
                                             
                    }
                }
            }

        }

        void calculate_live_range(std::vector<cfgNode> &graph,bool inter_block){

            live_range_vec initial_live_range,final_live_range;
            for(size_t node_num=0;node_num<graph.size();node_num++){

                for (auto itd=graph[node_num].def_set.begin(); itd!=graph[node_num].def_set.end(); ++itd){

                    int def_vec_len = itd->second.size();
                    //search for all the defintions of string
                    for(size_t i=0;i<def_vec_len;i++){

                        int matched = 0;
                        auto def_vec = itd->second[i];
                        if(inter_block) {
                            //search in all the blocks; starting from current block
                            for(size_t next_node=node_num; next_node<graph.size();next_node++){
                                
                                //serach in use set
                                for (auto itu=graph[next_node].use_set.begin(); itu!=graph[next_node].use_set.end(); ++itu){
                                
                                    if(itd->first == itu->first){
                                        matched = 1;
                                        int use_vec_len = itu->second.size();
                                        auto use_vec = itu->second[use_vec_len-1]; //take the line of last use
                                        live_range_details details;
                                        details.str_name = itd->first;
                                        details.start_line = def_vec.line_num;
                                        details.start_block = def_vec.block_num;
                                        details.end_line = use_vec.line_num;
                                        details.end_block = use_vec.block_num;
                                        
                                        initial_live_range.push_back(details);   
                                    }
                                    
                                }
                            }
                            //if no match is found in any of the use set then range is with in the line
                            if(matched == 0){
                                
                                live_range_details details;
                                details.str_name = itd->first;
                                details.start_line = def_vec.line_num;
                                details.end_line = def_vec.line_num;
                                details.start_block = def_vec.block_num;
                                details.end_block = def_vec.block_num;
                                initial_live_range.push_back(details);
                            }
                        }//end of interblock
                        else{

                            //serach in use set
                            for (auto itu=graph[node_num].use_set.begin(); itu!=graph[node_num].use_set.end(); ++itu){
                            
                                if(itd->first == itu->first){
                                    matched = 1;
                                    int use_vec_len = itu->second.size();
                                    auto use_vec = itu->second[use_vec_len-1]; //take the line of last use
                                    live_range_details details;
                                    details.str_name = itd->first;
                                    details.start_line = def_vec.line_num;
                                    details.start_block = def_vec.block_num;
                                    details.end_line = use_vec.line_num;
                                    details.end_block = use_vec.block_num;
                                    
                                    initial_live_range.push_back(details);   
                                }
                                
                            }
                            if(matched == 0){
                                
                                live_range_details details;
                                details.str_name = itd->first;
                                details.start_line = def_vec.line_num;
                                details.end_line = def_vec.line_num;
                                details.start_block = def_vec.block_num;
                                details.end_block = def_vec.block_num;
                                initial_live_range.push_back(details);
                            }
                        }
                    }
                    
                }

            }
            //print_live_range(initial_live_range);
            //this is same as creating webs of defs and use across basic blocks; according to the lecture;
            bool result = compare_live_range_vector(initial_live_range, final_live_range);
            
            while(result == false){

                live_range_vec input_live_range = initial_live_range;
                final_live_range.clear();
                converg_live_range_vector(graph,initial_live_range,final_live_range);

                result = compare_live_range_vector(input_live_range, final_live_range);
                initial_live_range = final_live_range;  
                 
            }
            print_live_range(final_live_range);
            live_range = final_live_range;
        }

        /* This function compares two vectors and return true if they are same otherwise false */
        bool compare_live_range_vector(live_range_vec &initial_live_range,live_range_vec &final_live_range)
        {
                
                int size1 = initial_live_range.size();
                int size2 = final_live_range.size();
        
                if(size1 != size2)
                    return false;

                int count = 0;
                for (int i =0;i<size1;i++){
                    for (int j =0;j<size2;j++){

                        if((initial_live_range[i].str_name == final_live_range[j].str_name) && 
                            (initial_live_range[i].start_block == final_live_range[j].start_block) &&
                            (initial_live_range[i].end_block == final_live_range[j].end_block) &&
                            (initial_live_range[i].start_line == final_live_range[j].start_line) &&
                            (initial_live_range[i].end_line == final_live_range[j].end_line)){

                                count++;
                        }
                    }
                }
                
                if(count == size1)
                    return true;
                else
                    return false;
        }

        /* This function tries to converge the live ranges  */
        void converg_live_range_vector(std::vector<cfgNode> &graph,live_range_vec &initial_live_range,live_range_vec &final_live_range)
        {
            //reiterate live range and refine it for the cases of multiple enties for same symbol
            bool change = true;
            
            while(change == true) {
                change = false;
                
                for(size_t node_num=0;node_num<initial_live_range.size();node_num++){

                    int matched = 0;
                    for(size_t next_node=node_num+1;next_node<initial_live_range.size();next_node++){
                     
                        /* if 2 ranges have same end point, then update endline and endblock for the live ranges */
                        if((initial_live_range[node_num].str_name == initial_live_range[next_node].str_name) && 
                            (initial_live_range[node_num].end_block == initial_live_range[next_node].end_block) &&
                            (initial_live_range[node_num].end_line == initial_live_range[next_node].end_line)){
                                
                                matched = 1;
                                change = true;
                                initial_live_range[node_num].end_line = initial_live_range[next_node].start_line-1;
                                initial_live_range[node_num].end_block = get_block_number(graph,initial_live_range[node_num].end_line);
                                final_live_range.push_back(initial_live_range[node_num]);
                                final_live_range.push_back(initial_live_range[next_node]);

                                initial_live_range.erase(initial_live_range.begin()+next_node);
                        }
                    }
                    /* if 2 ranges have same starting point, then consider the one having greater range */
                    for(size_t next_node=node_num+1;next_node<initial_live_range.size();next_node++){

                        if((initial_live_range[node_num].str_name == initial_live_range[next_node].str_name) && 
                            (initial_live_range[node_num].start_block == initial_live_range[next_node].start_block) &&
                            (initial_live_range[node_num].start_line == initial_live_range[next_node].start_line)){
                                                                
                                matched = 1;
                                change = true;
                                if(initial_live_range[next_node].end_line > initial_live_range[node_num].end_line){
                                    final_live_range.push_back(initial_live_range[next_node]);
                                }
                                else{
                                    final_live_range.push_back(initial_live_range[node_num]);
                                }
                                initial_live_range.erase(initial_live_range.begin()+next_node) ;
                        }
                    }
                    
                    if(matched)
                        initial_live_range.erase(initial_live_range.begin()+node_num);
                }
                
            }  
            
            int i = initial_live_range.size()-1;
            while(i >= 0){
                final_live_range.push_back(initial_live_range[i]);
                initial_live_range.erase(initial_live_range.begin()+i);
                i--;
            }
        }

        void print_live_range(live_range_vec ive_range){

            std::cout << "Live ranges are: " <<std::endl;
            std::cout << "variable" << "\t" << "start block" << "\t" << "end block" << "\t" << "start line" << "\t" << "end line" <<std::endl;
            std::cout << "***************************************************************"<<std::endl;
            for (auto it=ive_range.begin(); it!=ive_range.end(); ++it)
                std::cout << it->str_name << "\t\t" << it->start_block << "\t\t" << it->end_block << "\t\t" << it->start_line << "\t\t" << it->end_line  << '\n';
            std::cout << "***************************************************************"<<std::endl;

        }

        void allocate_register_intrablock(std::vector<cfgNode> &graph,live_range_vec &lrange){

            //we assume that we have 15 registers
            for(size_t node_num=0;node_num<graph.size();node_num++){

                std::vector<register_entry> usage_set;
                //create graph with nodes
                for (int i=0;i<lrange.size();i++){
                    if(node_num == lrange[i].start_block){
                        int usage_count = lrange[i].end_line - lrange[i].start_line;
                        usage_set.push_back(register_entry(usage_count,node_num,node_num,"",lrange[i].str_name));
                    }
                }

                //sort according to the maximum number of use
                std::sort(usage_set.begin(), usage_set.end(),sort_on_usage_count());
                int reg_count = 0;

                for (auto itu=usage_set.begin(); itu!=usage_set.end(); ++itu){
                    
                    if(reg_count < num_registers){

                        std::string reg = "reg" + std::to_string(reg_count);
                        register_map.push_back(register_entry(itu->usage_count,node_num,node_num,reg,itu->string_name));
                        //std::cout << itu->usage_count << " " << node_num+1 << " " <<reg << " " << itu->string_name<<std::endl;
                        reg_count++; 
                    }
                    else{

                        register_map.push_back(register_entry(itu->usage_count,node_num,node_num,"",itu->string_name));   
                    }
                } 
                
            }
            
        }
        
        void generate_final_ir_code(std::vector<cfgNode> &graph, std::vector<std::string> &ir_code,bool inter_block){

            for(size_t node_num=0;node_num<graph.size();node_num++){
            
                //load variables from this blocks register map
                for (auto itu=register_map.begin(); itu!=register_map.end(); ++itu){
                    
                    if(node_num == itu->start_block){
                        std::string str = "\tload_var " + itu->reg_name + " " + itu->string_name;
                        //std::cout << "\tload_var " << itu->reg_name<< " " << itu->string_name <<std::endl;
                        final_ir_code.push_back(str);
                    }
                }//end of initial load

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
                                              
                        std::string reg1="";
                        std::string reg2="";
                        std::string str;
                        std::string str1 = "assign";
                        bool res1 = find_in_reg_map(node_num,result[1],reg1,inter_block);
                        bool res2 = find_in_reg_map(node_num,result[2],reg2,inter_block);
                        if(!res1)
                            reg1 = result[1];
                        if(!res2)
                            reg2 = result[2];

                        //check result 2 in register map
                        str = "\t" + str1 + " " + reg1 + " " + reg2;
                        final_ir_code.push_back(str);
                        
                    }
                    else if((strcmp(result[0].c_str(),"assign") == 0) && (result.size()==4)){
                        
                        std::string reg1="";
                        std::string reg2="";
                        std::string reg3="";
                        std::string str;
                        std::string str1 = "assign";
                        bool res1 = find_in_reg_map(node_num,result[1],reg1,inter_block);
                        bool res2 = find_in_reg_map(node_num,result[2],reg2,inter_block);
                        bool res3 = find_in_reg_map(node_num,result[3],reg3,inter_block);
                        if(!res1)
                            reg1 = result[1];
                        if(!res2)
                            reg2 = result[2];
                        if(!res3)
                            reg3 = result[3];

                        str = "\t" + str1 + " " + reg1 + " " + reg2 + " " + reg3;
                        final_ir_code.push_back(str);
                    }
                    else if( (strcmp(result[0].c_str(),"brneq") == 0) || (strcmp(result[0].c_str(),"breq") == 0) 
                        || (strcmp(result[0].c_str(),"brlt") == 0) || (strcmp(result[0].c_str(),"brgt") == 0)
                        || (strcmp(result[0].c_str(),"brgeq") == 0) || (strcmp(result[0].c_str(),"brleq") == 0) ) {
                        
                        std::string reg1="";
                        std::string reg2="";
                        std::string str;
                        bool res1 = find_in_reg_map(node_num,result[1],reg1,inter_block);
                        bool res2 = find_in_reg_map(node_num,result[2],reg2,inter_block);
                        if(!res1)
                            reg1 = result[1];
                        if(!res2)
                            reg2 = result[2];

                        str = "\t" + result[0] + " " + reg1 + " " + reg2 + " " + result[3];
                        final_ir_code.push_back(str);

                    }
                    else if( (strcmp(result[0].c_str(),"add") == 0) || (strcmp(result[0].c_str(),"sub") == 0) 
                        || (strcmp(result[0].c_str(),"mult") == 0) || (strcmp(result[0].c_str(),"div") == 0)
                        || (strcmp(result[0].c_str(),"and") == 0) || (strcmp(result[0].c_str(),"or") == 0) ){

                        std::string reg1="";
                        std::string reg2="";
                        std::string reg3="";
                        std::string str;

                        bool res1 = find_in_reg_map(node_num,result[1],reg1,inter_block);
                        bool res2 = find_in_reg_map(node_num,result[2],reg2,inter_block);
                        bool res3 = find_in_reg_map(node_num,result[3],reg3,inter_block);

                        if(!res1)
                            reg1 = result[1];
                        if(!res2)
                            reg2 = result[2];
                        if(!res3)
                            reg3 = result[3];
                        
                        str = "\t" + result[0] + " " + reg1 + " " + reg2 + " " + reg3;
                       
                        final_ir_code.push_back(str);
                       
                    }
                    else if((strcmp(result[0].c_str(),"call") == 0) || (strcmp(result[0].c_str(),"callr") == 0)){
                        int offset = 0;
                        std::string reg1="";
                        std::string reg2="";
                        std::string str;
                        if(strcmp(result[0].c_str(),"callr") == 0){
                            offset = 1;
                            bool res1 = find_in_reg_map(node_num,result[1],reg1,inter_block);
                            if(res1){
                                std::string str1 = "callr";
                                str = "\t" + str1 + " " + reg1 + " " + result[2];
                            }
                        }
                        else{
                            std::string str1 = "call";
                            str = "\t" + str1 + " " + result[1];
                        }

                        for (size_t char_num =2+offset;char_num<result.size();char_num++){
                                
                            bool res2 = find_in_reg_map(node_num,result[char_num],reg2,inter_block); 
                            if(res2)
                                str +=  " " +  reg2;                
                        }
                        final_ir_code.push_back(str);
                    }
                    else if(strcmp(result[0].c_str(),"return") == 0){
                        std::string str;
                        if(result.size()==2){
                            std::string reg1="";
                            bool res1 = find_in_reg_map(node_num,result[1],reg1,inter_block);
                            
                            if(res1){
                                std::string str1 = "return";
                                str = "\t" + str1 +  " " + reg1;
                                final_ir_code.push_back(str);
                            }
                        }else{
                            std::string str1 = "return";
                            str = "\t" + str1;
                            final_ir_code.push_back(str);
                        }
                    
                    }
                    else{
                        //std::cout << "adding here......." << line <<std::endl;
                        final_ir_code.push_back(line);
                    }
                }
                //store variables from this blocks register to memory
                for (auto itu=register_map.begin(); itu!=register_map.end(); ++itu){
                    
                    if(node_num == itu->start_block){
                        std::string str = "\tstore_var " + itu->reg_name + " " + itu->string_name;
                        final_ir_code.push_back(str);
                    }
                }//end of initial load
            } 

        }

        bool find_in_reg_map(int block_num,std::string result,std::string& reg,bool inter_block){

            if(!inter_block){
                for (auto itu=register_map.begin(); itu!=register_map.end(); ++itu){
                        
                        if((block_num == itu->start_block) && (strcmp(result.c_str(),itu->string_name.c_str()) == 0)) {
                            reg = itu->reg_name;
                            return true;
                        }
                    }//end of initial load
            }
            else{

                for (auto itu=register_map.begin(); itu!=register_map.end(); ++itu){
                        
                        if(strcmp(result.c_str(),itu->string_name.c_str()) == 0) {
                            reg = itu->reg_name;
                            return true;
                        }
                    }//end of initial load
            }

            return false;
        }
        void print_ir_code(std::vector<std::string> &final_ir_code) {
          std::cout << "\n\n----------------------------------------" << std::endl;
          std::cout << "Generate Final IR CODE ..." << std::endl;
          std::cout << "----------------------------------------" << std::endl;
          for (size_t i = 0; i < final_ir_code.size(); ++i) {
            std::cout << final_ir_code[i] << std::endl;
          }
          std::cout << "----------------------------------------\n" << std::endl;
        }

        

        void  calculate_interference_graph(live_range_vec &lrange){

                //create graph with nodes
                for (int i=0;i<lrange.size();i++){
                    std::string node_name = "s" + std::to_string(i);
                    std::string var_name = lrange[i].str_name;
                    auto node = new interferenceGraphNode(node_name,var_name,lrange[i].start_line,lrange[i].end_line,lrange[i].start_block,lrange[i].end_block);
                    this->interferenceGraph.push_back(*node);
                }
                
                //add edges in the graph
                for(size_t node_num=0;node_num<interferenceGraph.size();node_num++){
                    for(size_t next_node=0;next_node<interferenceGraph.size();next_node++){

                        if(node_num != next_node){

                            if(((interferenceGraph[node_num].start_line >= interferenceGraph[next_node].start_line) &&
                                (interferenceGraph[node_num].start_line <= interferenceGraph[next_node].end_line)) ||
                                ((interferenceGraph[node_num].end_line >= interferenceGraph[next_node].start_line) &&
                                (interferenceGraph[node_num].end_line <= interferenceGraph[next_node].end_line)) ) {

                                interferenceGraph[node_num].addEdge(interferenceGraph[next_node]);
                                //std::cout << "adding edge from " << node_num << " " << next_node <<std::endl;
                            }

                        }
                    }

                }
                //calculate spill cost of each node; this is cost to have variable in the memory; equal to the times variable is used
                for(int node_num=0;node_num<interferenceGraph.size();node_num++){
                    //interferenceGraph[node_num].printEdges();
                    interferenceGraph[node_num].spill_cost = calculate_spill_cost(node_num);
                    //std::cout << "node is " << interferenceGraph[node_num].str_name << " " << interferenceGraph[node_num].spill_cost <<std::endl;
                }
                //sort and push in stack
                std::sort(interferenceGraph.begin(), interferenceGraph.end(),sort_on_spill_cost());
                //std::cout << "Var name" << " " << "node name" << " " << "spill cost" <<std::endl; 
                for(int node_num=0;node_num<interferenceGraph.size();node_num++){
                    interferenceStack.push(interferenceGraph[node_num].node_name);
                    //std::cout << interferenceGraph[node_num].var_name  << " " << interferenceGraph[node_num].node_name << " " << interferenceGraph[node_num].spill_cost<<std::endl;
                }
        }

        int calculate_spill_cost(int in_node){

            int spill_cost = 0;
            for(size_t node_num=0;node_num<interferenceGraph.size();node_num++){
                if(node_num == in_node){
                    spill_cost = interferenceGraph[node_num].end_line - interferenceGraph[node_num].start_line;
                }
            }
            return spill_cost;
        }

        void update_avilable_colors(std::string input_str){

            
            for(size_t node_num=0;node_num<interferenceGraph.size();node_num++){
                //std::cout << "Input string is " <<interferenceGraph[node_num].str_name << " " << input_str <<std::endl;
                if(strcmp(interferenceGraph[node_num].node_name.c_str(),input_str.c_str()) == 0){
                    
                    //check out going edges and remove the color of those nodes from avilable colors
                    for (int j=0;j<interferenceGraph[node_num].edges.size();j++){
                    
                        for(size_t next_node=0;next_node<interferenceGraph.size();next_node++){

                            if(strcmp(interferenceGraph[node_num].edges[j].node_name.c_str(),interferenceGraph[next_node].node_name.c_str()) == 0){

                                //std::cout << "checking out going edge " << interferenceGraph[node_num].str_name << " " << interferenceGraph[next_node].str_name \
                                << " " << interferenceGraph[next_node].assigned_color <<std::endl;
                                if(interferenceGraph[next_node].assigned_color != -1){

                                    int to_del = interferenceGraph[next_node].assigned_color;
                                    interferenceGraph[node_num].erase_color(to_del);
                                }
                            }
                        }
                    }

                }
            }
            for(size_t node_num=0;node_num<interferenceGraph.size();node_num++){ //check incoming edges to the node

                if(strcmp(interferenceGraph[node_num].node_name.c_str(),input_str.c_str()) == 0){

                    for(size_t next_node=0;next_node<interferenceGraph.size();next_node++){

                        for (int j=0;j<interferenceGraph[next_node].edges.size();j++) {

                            if(strcmp(interferenceGraph[next_node].edges[j].node_name.c_str(),interferenceGraph[node_num].node_name.c_str()) == 0){

                                //std::cout << "checking in coming edge " << interferenceGraph[node_num].str_name << " " << interferenceGraph[next_node].str_name \
                                << " " << interferenceGraph[next_node].assigned_color <<std::endl;

                                if(interferenceGraph[next_node].assigned_color != -1){
                                    
                                    int to_del = interferenceGraph[next_node].assigned_color;
                                    interferenceGraph[node_num].erase_color(to_del);
                                }
                            }
                        }
                        
                    }
                }

            }
            
        }
        void update_node_color(){

            while (!interferenceStack.empty()){


                std::string str = interferenceStack.top();
                interferenceStack.pop();
                update_avilable_colors(str);
                
                for(size_t node_num=0;node_num<interferenceGraph.size();node_num++){
    
                    if(strcmp(interferenceGraph[node_num].node_name.c_str(),str.c_str()) == 0){
                        //assign the first avilable color to the node
                        if(interferenceGraph[node_num].avilable_colors.size() > 0){

                            interferenceGraph[node_num].assigned_color = interferenceGraph[node_num].avilable_colors[0];
                            interferenceGraph[node_num].assign_register(interferenceGraph[node_num].assigned_color);  
                            register_map.push_back(register_entry(0,interferenceGraph[node_num].start_block,interferenceGraph[node_num].end_block,
                                interferenceGraph[node_num].assigned_reg,interferenceGraph[node_num].var_name));
                            //std::cout << "assigning color to " << interferenceGraph[node_num].var_name << " " << interferenceGraph[node_num].assigned_reg <<std::endl;
                        }
                        else { //this is the case of spill

                                interferenceGraph[node_num].isSpillEnable = true;
                                register_map.push_back(register_entry(0,interferenceGraph[node_num].start_block,interferenceGraph[node_num].end_block,
                                    "",interferenceGraph[node_num].var_name));  
                        }

                    }
                }

            }

        }

        void update_intra_reg_allocation() {
            
            //create and print edges
            create_cfg_edges(graph, ir_code);
            for(int node_num=0;node_num<graph.size();node_num++){
                graph[node_num].printEdges();
            }
            calculate_def_use_set(graph, ir_code);
            for(int node_num=0;node_num<graph.size();node_num++){
                graph[node_num].print_du(node_num);
            }
            calculate_live_range(graph,false);
            allocate_register_intrablock(graph,live_range);
            generate_final_ir_code(graph, ir_code,false);
            print_ir_code(final_ir_code);
        }

    
        void update_inter_reg_allocation() {
            
            //create and print edges
            create_cfg_edges(graph, ir_code);
            for(int node_num=0;node_num<graph.size();node_num++){
                graph[node_num].printEdges();
            }
            calculate_def_use_set(graph, ir_code);
            for(int node_num=0;node_num<graph.size();node_num++){
                graph[node_num].print_du(node_num);
            }

            calculate_live_range(graph,true);
            calculate_interference_graph(live_range);
            update_node_color();
            generate_final_ir_code(graph, ir_code,true);
            print_ir_code(final_ir_code);
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
  graph.update_intra_reg_allocation();

  cfg graph1(argv[1]);
  graph1.createBasicBlocks();
  graph1.update_inter_reg_allocation();

  return 0;
}

# -*- coding: utf-8 -*-
"""
Created on Fri Feb 10 07:39:49 2017

@author: rachna.s
"""
import numpy as np
rule_list=[]
terminal_list = []
predicted=[]
non_terminal_list=[]
first_set_list = []
follow_set_list = []

class rule:
    def __init__(self):
        self.lhs = []
        self.rhs = []
        
    def print_rule(self):
        print self.lhs,self.rhs
        
def read_input_file(filename):
    f = open(filename, 'r')
    line_num = 0
    for line in f:
        r_obj = rule()
        ll = []
        lsplit = line.split(" ")
        len_lsplit = len(line.split())
        i=0
        while(len_lsplit>0):
            ll.append(lsplit[i])
            len_lsplit -=1
            i +=1
        ll[-1]=ll[-1].rstrip() #remove \n from the last word
        
        if(line_num > 0):
            list_of_lhs = [r1.lhs for r1 in rule_list]
            result = [element for element in list_of_lhs if element[0] == ll[0]]
            if(result):
                for r in rule_list:
                    if(r.lhs == result[0]):
                        r.rhs.append(ll[1:])
            else:
                r_obj.lhs.append(ll[0])
                r_obj.rhs.append((ll[1:]))
                rule_list.append(r_obj)
        else:
            r_obj.lhs.append(ll[0])
            r_obj.rhs.append((ll[1:]))
            rule_list.append(r_obj)
        line_num += 1
        #r.print_rule()

    t = [(r1.lhs,r1.rhs) for r1 in rule_list]
    print t, len(t)
    f.close()

def create_terminal_list():

    t = [(r.lhs,r.rhs) for r in rule_list]
    lhs_list = [r.lhs for r in rule_list]
    non_terminal_list.append((lhs_list[0][0]))
    for i in range(0,len(t)):
        #print t[i][0],t[i][1],len(t[i][1])
        rhs = t[i][1]
        for j in range(0,len(rhs)):
            for k in range(0,len(rhs[j])):
                found =0
                #print rhs[j][k], lhs_list
                for l in lhs_list:
                    #print rhs[j][k], l
                    if(rhs[j][k] in l):
                        found = 1
                        #print "making found 1"
                if((found ==0)and((rhs[j][k] not in terminal_list))):
                    terminal_list.append(rhs[j][k])
                if((found ==1)and((rhs[j][k] not in non_terminal_list))):
                    non_terminal_list.append(rhs[j][k])
    #print "terminal list is: ", terminal_list
    #print "non-terminal list is: ",non_terminal_list
def check_has_epsilon(l):
    got_epsilon = 0
    ret_index = -1
    for i in range(0,len(l[1])):
        if(l[1][i] == 'epsilon'):
            got_epsilon = 1
            ret_index = i
    return got_epsilon, ret_index

def find_first_set(element):
    #print "input is ---------------", element
    if element in terminal_list:
        #print "This is terminal node ....",element,element
        return (element,[element])
    elif(first_set_list):
        #print first_set_list
        fset_tup = [tup for tup in first_set_list if tup[0]==element]
        if(fset_tup):
            #print "Already in the list ....",fset_tup[0]
            return fset_tup[0]

    t = [(r1.lhs,r1.rhs) for r1 in rule_list]
    for i in range(0,len(t)):
        if(t[i][0][0] == element):
            local_list = []
            list_rhs = t[i][1]
            for j in range(0,len(list_rhs)):
                k = 0
                got_epsilon = 0
                while(k < len(list_rhs[j])):
                    if(list_rhs[j][k] in non_terminal_list):
                        t = find_first_set(list_rhs[j][k])
                        got_epsilon,ret_index = check_has_epsilon(t)
                        if(got_epsilon):
                            tt = t[1][:ret_index]
                            for t in range(0,len(tt)):
                                local_list.append(tt[t])
                        else:
                            tt = t[1][:]
                            for t in range(0,len(tt)):
                                local_list.append(tt[t])
                            break
                    else:
                        e, l = find_first_set(list_rhs[j][k])
                        local_list.append(e)
                        break
                    k = k+1

            if((got_epsilon)and('epsilon' not in local_list)):
                local_list.append('epsilon')
            #print "Returning...",element,local_list
            #print "First set returning....", element,local_list
            return (element,local_list)
    #first_set_list.append((element,[]))
    #print first_set_list

def check_if_nullable(element):

    got_epsilon = 0
    t = [(r1.lhs,r1.rhs) for r1 in rule_list]
    for i in range(0,len(t)):
        list_rhs = t[i][1]
        lhs = t[i][0]
        if(lhs[0] == element):
            for j in range(0,len(list_rhs)):
                for k in range(0,len(list_rhs[j])):
                    if(list_rhs[j][k] == 'epsilon'):
                        got_epsilon = 1
                        break

    return  got_epsilon

def find_follow_set(element):
    #print "input is ---------------", element

    if(element in terminal_list):
        return []
    if(follow_set_list):
        fset_tup = [tup for tup in follow_set_list if tup[0]==element]
        if(fset_tup):
            #print "Already in the list ....",fset_tup[0]
            return fset_tup[0]

    list_to_return = []
    t = [(r1.lhs,r1.rhs) for r1 in rule_list]
    for i in range(0,len(t)):
        list_rhs = t[i][1]
        lhs = t[i][0]
        for j in range(0,len(list_rhs)):
            for k in range(0,len(list_rhs[j])):
                if(list_rhs[j][k] == element):
                    got_epsilon = 0
                    if(k == (len(list_rhs[j]) -1)):
                        if(list_rhs[j][k] != lhs[0]):
                            got_epsilon = 1#check_if_nullable(list_rhs[j][k])
                    else:
                        n = k+1
                        while(n < (len(list_rhs[j]))):
                            first_set = find_first_set(list_rhs[j][n])
                           # print "first set ..........",first_set,len(first_set[1])
                            got_epsilon = 0
                            for m in range(0,len(first_set[1])):
                                if(first_set[1][m]!="epsilon"):
                                    if(first_set[1][m] not in list_to_return):
                                        list_to_return.append(first_set[1][m])
                                else:
                                    got_epsilon = 1
                            if(got_epsilon==0):
                                break
                            n= n + 1

                    if(got_epsilon or(len(list_to_return) == 0)):
                        #print "calling follow set again...",lhs[0]
                        e,l=find_follow_set(lhs[0])
                        for m in range(0,len(l)):
                            if(l[m] not in list_to_return):
                                list_to_return.append(l[m])
                        #print "here list is ", list_to_return

    #print "Returning .......", list_to_return
    return (element,list_to_return)

def find_predict():
    #print "######################################################33"
    t = [(r1.lhs,r1.rhs) for r1 in rule_list]
    #print t, len(t)

    for i in range(0,len(t)):
        list_rhs = t[i][1]
        lhs = t[i][0]
        #print "RHS ", list_rhs
        for j in range(0,len(list_rhs)):
            got_epsilon = 0
            local_list = []
            #print "starting................",list_rhs[j][0], len(list_rhs), list_rhs[j]
            for k in range(0,len(list_rhs[j])):
                first_set = get_from_first_set(list_rhs[j][k])
                #print "first set", first_set
                if(first_set):
                    for sublist in first_set:
                        if(sublist != "epsilon"):
                            local_list.append(sublist)
                            #print "local list is ", local_list
                        else:
                            got_epsilon = 1
                if(got_epsilon == 0):
                    break
            if(got_epsilon==1):
                follow_set = get_from_follow_set(lhs[0])
                #print "follow set", follow_set
                if(follow_set):
                    for sublist in follow_set:
                        local_list.append(sublist)


            t1 = t[i][0]+t[i][1][j]
            #print t[i][0], t[i][1][j], t1
            predicted.append((t1,local_list))
            #print "At last--------------",(t1,local_list)

    print "Predicted set is :"
    print predicted

def  get_from_first_set(element):

    for tup in first_set_list:
        #print tup[0], tup[1]
        if (tup[0]==element):
            return tup[1]
    return []

def  get_from_follow_set(element):

    for tup in follow_set_list:
        #print tup[0], tup[1]
        if (tup[0]==element):
            return tup[1]
    return []

def generate_parser_table():
    print "############################################"
    size_ter= len(terminal_list)
    size_non_ter = len(non_terminal_list)
    idx = 0
    for i, j in enumerate(terminal_list):
        if j == 'epsilon':
            idx = i

    modified_ter_list = terminal_list[:]
    del modified_ter_list[idx]
    modified_ter_list.append('$')

    parser_table = np.empty((size_non_ter, size_ter))
    parser_table[:] = np.NAN

    print "terminal list = ", terminal_list, len(terminal_list)
    print "modified terminal list is ", modified_ter_list, len(modified_ter_list)
    print "non-terminal list = ", non_terminal_list, len(non_terminal_list)
    tup_index = 0
    print "Rule list is: "
    for tup in predicted:
        lhs = tup[0]
        rhs = tup[1]
        print tup_index, lhs, rhs
        for i in range(0,size_non_ter):#row index non-terminal list
            if(lhs[0] == non_terminal_list[i]):
                for j in range(0,size_ter):#col index for terminal list
                    for k in range(0,len(rhs)):
                        if(rhs[k] == modified_ter_list[j]):
                            parser_table[i][j] = tup_index

        tup_index =tup_index+1
    print "Final parser table: rows->non-terminal, col->modified terminals. index starts with 0. Integer represents rule index in rule list"
    print('\n'.join([''.join(['{:8}'.format(item) for item in row])
      for row in parser_table]))

if __name__ == '__main__':

    read_input_file("Tiger_grammar.txt")
    #read_input_file("grammer_hw2.txt")
    create_terminal_list()
    ntl = non_terminal_list[:]
    #ntl.reverse()

    #########################first set#######################

    for i in range(0,len(terminal_list)):
        tup = find_first_set(terminal_list[i])
        first_set_list.append(tup)
        print "First ",tup

    for i in range(0,len(ntl)):
        #print "calling for............",ntl[i]
        tup = find_first_set(ntl[i])
        first_set_list.append(tup)
        print "First ",tup
    #print "First set is: ", first_set_list

    ######################follow set########################

    t = [(r1.lhs,r1.rhs) for r1 in rule_list]

    #follow_set_list.append((t[0][0][0],['$']))
    '''
    for i in range(0,len(terminal_list)):
        tup = find_follow_set(terminal_list[i])
        follow_set_list.append(['NA'])
    '''
    ntl = non_terminal_list[:]
    for i in range(0,len(ntl)):
        e, l = find_follow_set(ntl[i])
        #print "got back *******************************", e,l, follow_set_list
        if(i == 0):
            l.append('$')
            follow_set_list.append((e,l))
        else:
            follow_set_list.append((e,l))
        print "Follow ", e,l
    #print "Follow set is: ",follow_set_list

    ############################find predict######################
    find_predict()
    generate_parser_table()

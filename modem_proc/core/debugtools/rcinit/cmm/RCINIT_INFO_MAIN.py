#============================================================================
#  Name:
#    RCINIT_INFO_MAIN.py
#
#  Description:
#    This script is used to extract RCINIT function and task related information.
# 
#  Copyright (c) 2020 by Qualcomm Technologies, Inc.  All Rights Reserved.
#  Qualcomm Confidential and Proprietary
#----------------------------------------------------------------------------
#============================================================================


import xlsxwriter
import csv
import os
filename = raw_input("Please enter a unique string (This will be used in the filename of xlsx file):\n")

def input_validation():
    chars = set('@#$%^&*-+=/\;,.<>?|~`":!{}[]()')
    if any((c in chars) for c in filename):
        print('\nFile name not valid.')
        print('Do not use any of these characters in file name.')
        for i in chars:
            print i,
        exit()

input_validation()
filename="RCINIT_INFO_"+filename
filepath="C:\Temp"

workbook = xlsxwriter.Workbook(filepath+"\\"+filename+'.xlsx')
summary=workbook.add_worksheet('SUMMARY')
group0=workbook.add_worksheet('GROUP 0')
group1=workbook.add_worksheet('GROUP 1')
group2=workbook.add_worksheet('GROUP 2')
group3=workbook.add_worksheet('GROUP 3')
group4=workbook.add_worksheet('GROUP 4')
group5=workbook.add_worksheet('GROUP 5')
group6=workbook.add_worksheet('GROUP 6')
group7=workbook.add_worksheet('GROUP 7')
groups=[group0,group1,group2,group3,group4,group5,group6,group7]

rcinit_groups={
0:[[],[],0,[]],1:[[],[],0,[]],2:[[],[],0,[]],3:[[],[],0,[]],4:[[],[],0,[]],5:[[],[],0,[]],6:[[],[],0,[]],7:[[],[],0,[]],
}

process_state=None
group_current=None
function_current=None

cell_format = workbook.add_format()
cell_format_header = workbook.add_format()
init_functions_ok=0
init_tasks_ok=1
#rcinit_groups -> key=group_number, values=functions_info, tasks_info, group_time, handshake_notgiven
#functions--(appended with group func time),tasks---(appended with group task time),grouptime(in time tick),incomplete tasks,
#functions=group,name,time_ms,timetick
#tasks=group,name,time_ms,timetick,handshake
def set_format():
    cell_format.set_bold()
    cell_format.set_font_color('black')
    cell_format.set_align('center')
    cell_format.set_align('vcenter')
    
    cell_format_header.set_font_size(13)
    cell_format_header.set_bold()
    cell_format_header.set_font_color('red')
    cell_format_header.set_align('center')
    
def process_functions(file,rcinit_groups):
    with open(file,'r') as readfile:
        reader = csv.reader(readfile)
        for i in reader:
            rcinit_groups[int(i[0],16)][0].append(i)
    
    for i in rcinit_groups.values():
        temp=0
        for j in i[0]:
            j[0]=int(j[0],16)
            j[2]=int(j[2],16)
            j[3]=int(j[3],16)
            temp=temp+j[3]
        i[0].append(["TotalFuncTime",temp])
        
    for i in rcinit_groups:
        number_of_functions=len(rcinit_groups[i][0])
        k=1        
        for j in rcinit_groups[i][0]:
            if(len(j) > 2):
                groups[i].write(k,0,j[1],cell_format)
                groups[i].write(k,1,j[2],cell_format)
                groups[i].write(k,2,j[3],cell_format)
                k=k+1

def process_tasks(file,rcinit_groups):
    with open(file,'r') as readfile:
        reader = csv.reader(readfile)
        for i in reader:
            rcinit_groups[int(i[0],16)][1].append(i)
            
    for i in rcinit_groups.values():
        temp=0
        for j in i[1]:
            j[0]=int(j[0],16)
            j[2]=int(j[2],16)
            j[3]=int(j[3],16)
            j[4]=int(j[4],16)
            if(j[4]==0):
                i[3].append(j[1])
            temp=temp+j[3]
        i[1].append(["TotalTaskTime",temp])
        
    for i in rcinit_groups:
        number_of_tasks=len(rcinit_groups[i][1])
        k=1        
        for j in rcinit_groups[i][1]:
            if(len(j) > 2):
                groups[i].write(k,6,j[1],cell_format)
                groups[i].write(k,7,j[2],cell_format)
                groups[i].write(k,8,j[3],cell_format)
                groups[i].write(k,9,j[4],cell_format)
                k=k+1
            
def process_grouptime(file,rcinit_groups):
    temp=[]
    with open(file,'r') as readfile:
        reader = csv.reader(readfile)
        for i in reader:
            temp.append(i)
    temp=[int(i[0],16) for i in temp]
    for i in range(0,8):
        rcinit_groups[i][2]=temp[i+1]-temp[i]
    global process_state
    process_state=temp[9]
    global group_current
    group_current=temp[10]    
    global function_current
    function_current=temp[11]
    del temp

def prepare_summary():
    summary.set_column(0, 1, 34)
    summary.set_column(2, 2, 9)
    summary.set_column(3, 4, 54)
    summary.set_column(5, 5, 9)
    summary.set_column(6, 6, 43)
    summary.write(0,0,'NUMBER OF FUNCTIONS IN GROUP 0 ',cell_format)
    summary.write(0,1,'NUMBER OF TASKS IN GROUP 0 ',cell_format)
    summary.write(0,3,'TOTAL TIME OF FUNCTIONS IN GROUP 0 (In micro seconds)',cell_format)
    summary.write(0,4,'TOTAL TIME OF TASKS IN GROUP 0 (In micro seconds)',cell_format)
    summary.write(3,0,'NUMBER OF FUNCTIONS IN GROUP 1 ',cell_format)
    summary.write(3,1,'NUMBER OF TASKS IN GROUP 1 ',cell_format)
    summary.write(3,3,'TOTAL TIME OF FUNCTIONS IN GROUP 1 (In micro seconds)',cell_format)
    summary.write(3,4,'TOTAL TIME OF TASKS IN GROUP 1 (In micro seconds)',cell_format)   
    summary.write(6,0,'NUMBER OF FUNCTIONS IN GROUP 2 ',cell_format)
    summary.write(6,1,'NUMBER OF TASKS IN GROUP 2 ',cell_format)
    summary.write(6,3,'TOTAL TIME OF FUNCTIONS IN GROUP 2 (In micro seconds)',cell_format)
    summary.write(6,4,'TOTAL TIME OF TASKS IN GROUP 2 (In micro seconds)',cell_format)
    summary.write(9,0,'NUMBER OF FUNCTIONS IN GROUP 3 ',cell_format)
    summary.write(9,1,'NUMBER OF TASKS IN GROUP 3 ',cell_format)
    summary.write(9,3,'TOTAL TIME OF FUNCTIONS IN GROUP 3 (In micro seconds)',cell_format)
    summary.write(9,4,'TOTAL TIME OF TASKS IN GROUP 3 (In micro seconds)',cell_format)
    summary.write(12,0,'NUMBER OF FUNCTIONS IN GROUP 4 ',cell_format)
    summary.write(12,1,'NUMBER OF TASKS IN GROUP 4 ',cell_format)
    summary.write(12,3,'TOTAL TIME OF FUNCTIONS IN GROUP 4 (In micro seconds)',cell_format)
    summary.write(12,4,'TOTAL TIME OF TASKS IN GROUP 4 (In micro seconds)',cell_format)
    summary.write(15,0,'NUMBER OF FUNCTIONS IN GROUP 5 ',cell_format)
    summary.write(15,1,'NUMBER OF TASKS IN GROUP 5 ',cell_format)
    summary.write(15,3,'TOTAL TIME OF FUNCTIONS IN GROUP 5 (In micro seconds)',cell_format)
    summary.write(15,4,'TOTAL TIME OF TASKS IN GROUP 5 (In micro seconds)',cell_format)
    summary.write(18,0,'NUMBER OF FUNCTIONS IN GROUP 6 ',cell_format)
    summary.write(18,1,'NUMBER OF TASKS IN GROUP 6 ',cell_format)
    summary.write(18,3,'TOTAL TIME OF FUNCTIONS IN GROUP 6 (In micro seconds)',cell_format)
    summary.write(18,4,'TOTAL TIME OF TASKS IN GROUP 6 (In micro seconds)',cell_format)
    summary.write(21,0,'NUMBER OF FUNCTIONS IN GROUP 7 ',cell_format)
    summary.write(21,1,'NUMBER OF TASKS IN GROUP 7 ',cell_format)
    summary.write(21,3,'TOTAL TIME OF FUNCTIONS IN GROUP 7 (In micro seconds)',cell_format)
    summary.write(21,4,'TOTAL TIME OF TASKS IN GROUP 7 (In micro seconds)',cell_format)
    summary.write(0,6,"Total time taken by GROUP 0 (In micro seconds)",cell_format)
    summary.write(3,6,"Total time taken by GROUP 1 (In micro seconds)",cell_format)
    summary.write(6,6,"Total time taken by GROUP 2 (In micro seconds)",cell_format)
    summary.write(9,6,"Total time taken by GROUP 3 (In micro seconds)",cell_format)
    summary.write(12,6,"Total time taken by GROUP 4 (In micro seconds)",cell_format)
    summary.write(15,6,"Total time taken by GROUP 5 (In micro seconds)",cell_format)
    summary.write(18,6,"Total time taken by GROUP 6 (In micro seconds)",cell_format)
    summary.write(21,6,"Total time taken by GROUP 7 (In micro seconds)",cell_format)
    
def prepare_group_sheets():
    for i in groups:
        i.set_column(0, 0, 38)
        i.set_column(1, 1, 30)
        i.set_column(2, 2, 10)
        i.set_column(6, 6, 38)
        i.set_column(7, 7, 30)
        i.set_column(8, 8, 10)
        i.set_column(9, 9, 13)
        i.write(0,0,'FUNCTION NAME',cell_format)
        i.write(0,1,'TOTAL TIME (In micro seconds)',cell_format)
        i.write(0,2,'TIMETICK',cell_format)
        i.write(0,6,'TASK NAME',cell_format)
        i.write(0,7,'TOTAL TIME (In micro seconds)',cell_format)
        i.write(0,8,'TIMETICK',cell_format)
        i.write(0,9,'HANDSHAKE',cell_format)

def write_summary():
    k=1
    for i in rcinit_groups:
        summary.write(k,0,len(rcinit_groups[i][0])-1,cell_format)
        summary.write(k,1,len(rcinit_groups[i][1])-1,cell_format)
        summary.write(k,3,rcinit_groups[i][0][-1][1]/19.2,cell_format)
        summary.write(k,4,rcinit_groups[i][1][-1][1]/19.2,cell_format)
        if(rcinit_groups[i][2]/19.2 >= 0):
            summary.write(k,6,rcinit_groups[i][2]/19.2,cell_format)
        else:
            summary.write(k,6,0,cell_format)
        k=k+3
        
    if(init_functions_ok and init_tasks_ok):
        summary.write(28,3,"RCINIT IS FINISHED",cell_format_header)
    else:
        summary.write(28,3,"RCINIT IS NOT FINISHED",cell_format_header)
        if(not init_functions_ok):
            summary.write(29,0,"Current Function ---->",cell_format_header)
            summary.write(29,1,function_current,cell_format_header)
        if(not init_tasks_ok):
            summary.write(30,0,"Handshake not received from --->",cell_format_header)
            step=1
            for i in rcinit_groups:
                for j in (rcinit_groups[i][3]):
                    summary.write(30,step,j,cell_format_header)
                    step=step+1
        
def is_rcinit_ok():
    global init_functions_ok
    global init_tasks_ok
    global function_current
    global group_current
    global process_state
    
    if(function_current==0 and group_current==8 and process_state==255):
        init_functions_ok=1
    else:
        last=[]
        for i in range(0,8):
            if(len(rcinit_groups[i][0])>1):
                last=(rcinit_groups[i][0][-2])
        function_current=(last[1])
        
    for i in rcinit_groups:
        if(len(rcinit_groups[i][3])>0):
            init_tasks_ok=0
    
    print("Init Functions ok = ",init_functions_ok)
    print("Init Tasks ok = ",init_tasks_ok)
        
def cleanup():
    os.remove("C:\Temp\initFunc.csv")
    os.remove("C:\Temp\initTask.csv")
    os.remove("C:\Temp\initTime.csv")

def main():
    set_format()
    process_functions('C:\Temp\initFunc.csv',rcinit_groups)
    process_tasks('C:\Temp\initTask.csv',rcinit_groups)
    process_grouptime('C:\Temp\initTime.csv',rcinit_groups)
    prepare_group_sheets()
    prepare_summary()
    is_rcinit_ok()
    write_summary()
    cleanup()

main()
workbook.close()
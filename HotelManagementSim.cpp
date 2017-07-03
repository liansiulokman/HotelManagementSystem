// ************************************************
	// Name : Siu Lok Man Lian
	// Student ID : 14040502D
	// Date : 1/4/2015
	// Program Name: Hotel Management System
	// Description of the program: This program is about the simulation on the basic operations on hotel management and operations. The program runs like a front desk
	//							   in a hotel and can serve both customers and admins in different operations. Administrators are allowed to view the internal information 
	//							   of the hotel or provide services for customers, while the customers can only choose the service provided. 
	//							   Since this program is just a simulation of a hotel, the services provided are limited and there will be less complex factors affecting
	//							   the hotel management environment. To facilitate the debugging process in building this program and the understanding of the users, 
	//							   assumptions are made and they may not be very close to reality. However, this program is as flexible as possible in order to facilitate
	//							   its maintenance efficiency with lowest time cost. The details of assumptions are stated clearly below and also in the report.
	//*************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>//for performing string functions
#include <time.h>//for the algorithm in picking room numbers randomly
#include <conio.h>//available in Windows platform only
#include <ctype.h>//for checking if input is a digit
#define _CRT_SECURE_NO_WARNINGS

//Assumptions: 
//****Since the hotelSim.txt and customerSim.txt has been used for testing for many times, some room numbers which is originally in luxury type may changed to standard type.
//1. There are already some customers checked-in and the details of the customer are stored in customerSim.txt. 
//2. To load the customer information into the program, user should set the currentPath as the file location of the customerSim.txt.
//3. The swapPath should be set in similar name with customerSim.txt and stored in the same file location with customerSim.txt.

//4. Hotel's information are initialized at line 85, which can be modified by the users.
//5. To load the updated hotel rooms' information, user should set the hotelPath as the file location which contains hotelSim.txt
//6. To facilitate testing and debugging, the number of hotel rooms are assumed and some rooms are already occupied by customers.

//7. Initially, half of the total rooms will be standard type and the other half will be in luxury type
//8  Say in the assumed case, the total room will be 20, 10 for standard type and 10 for luxury type.
//9. The maximum number of rooms are assumed to be 100, which can be modified by setting the sizes of the arrays in the structure of Hotel as the new total number of rooms
//10.All room numbers counts from 101 (the first room) to 200 (the 100th room), assumed the maximum number of rooms is 100.
//11.New customer can choose the required room type and bed type if rooms and beds are available, if number of available rooms are more than 1, the room numbers will be generated randomly.
//12.Room numbers of standard room type came first before those in luxury type initially, but won't limit the new rooms built.
//13.In this assumed case, room 101-110 will be in standard type and 111-120 will be in luxury type.
//14.Since the bed types can't changed rapidly in a same room, it is assumed that all rooms with even room numbers are with a twin bed, those with odd room numbers are with two single beds.
//15.If the total number of rooms in the hotel is an odd number, the numbers of standard type rooms will be higher.

//e.g. 20 rooms in total:
//	   10 for standard, 10 for luxury, room numbers: 101-110 (standard) and 111-120 (luxury).
//	   
//e.g. 15 rooms in total:
//	   8 for standard, 7 for luxury, room numbers: 101-108 (standard) and 109-115 (luxury).

//global declarations
bool quit=false;//set variable named quit to indicate the program has not stopped by the user yet
char stars[]="*************************************************";//storing the formatted printing pattern
int customerCount=0;//variable to store the updated number of customer through out the whole program
char currentPath[200]="C:\\customerSim.txt";//set as the path of the most updated file containing customer information
char swapPath[200];//the variable storing another path for writing another updated file with customer information temporarily
char hotelPath[200]="C:\\hotelSim.txt";//set as the path of the most updated file containing information of the hotel rooms


//set up the structure of storing hotel information
struct Hotel{//storing hotel information
	char name[30];//storing the hotel name for display in hotel information
	char address[100];//storing the hotel address for display in hotel information

	int totalRoom;//number of total rooms
	int totalLuxury;//number of luxury rooms
	int totalStandard;//number of standard rooms

	int roomAvailable;//number of rooms available
	int standardAvailable;////number of standard rooms available
	int luxuryAvailable;////number of luxury rooms available

	int roomNumbers[100];//array contains room numbers
	int luxuryRoomNumbers[100];//array contains luxury room numbers
	int standardRoomNumbers[100];//array contains standard room numbers

	int availableRooms[100];//array contains available room numbers
	int availableLuxury[100];//array contains available rooms in Luxury type
	int availableStandard[100];//array contains available rooms in Standard type

	int occupied[100];//array contains occupied room numbers
	char restaurant[30];//storing the restaurant name in the hotel for display in hotel information
};

//modify the hotel name, address and restaurant name here
//the room numbers and rooms' information will be updated by hotelSim.txt
struct Hotel h={"Hello World Hotel","123 Poly Rd, HK.",20,10,10,20,10,10,{},{},{},{},{},{},{},"Hello C Restaurant"};

//set up the structure of storing customers' information
struct Customer{//storing customer information
	char name[20];//storing customer's name
	char address[100];//storing customer's address
	char id[50];//storing customer's ID/passport number
	char date[10];//storing customer's check-in date
	struct Room{//storing details of customer's room
		char type[10];//storing room type
		char number[5];//storing room number
		float fee;//storing room fee
		float charges;//storing extra charges
		char bed[20];//storing bed type
	}room;
}c[100];//assumed maximum hotel room numbers is 100, therefore maximum customer record is also 100 (can be modified)

int customerWrite(){//update the customer record in the customerSim.txt file as final destination when program terminates normally
	FILE *fptr;//create file pointer for writing
	fptr=fopen("C:\\customerSim.txt","w");//the path should be same as the source path of customer text file used at the beginning of the program
	for(int q=0;q<customerCount;q++){//loop n times, n refers to the customerCount which is the current number of customers in hotel
		fprintf(fptr,"%s#",c[q].name);//write customer's name
		fprintf(fptr,"%s#",c[q].address);//write customer's address
		fprintf(fptr,"%s#",c[q].id);//write customer's id/passport
		fprintf(fptr,"%s#",c[q].date);//write customer's check-in date
		fprintf(fptr,"%s#",c[q].room.number);//write customer's occupied room number
		fprintf(fptr,"%.2f#",c[q].room.charges);//write customer's accumulated extra charges if customer took some services like dine, ordering wine, phone call service, club entrance, etc
		fprintf(fptr,"%.2f\n",c[q].room.fee);//write the customer's room rental fee only, which is optional as it will be always zero, room fee only calculated during check-out but not increment daily as this program do not provide real time counting on days
	}
	fclose(fptr);//close file pointer
	return 0;
}

int sortArray(int arr[100], int total){//function to sort the room numbers before writing into the text file
	//sort 
	for(int k=0;k<(total-1);k++){//loop k times
		for(int j=0;j<(total-1);j++){//loop j times
			if(arr[j+1]<arr[j]){//if the next number is smaller, swap their positions
				int tem;
				tem=arr[j+1];
				arr[j+1]=arr[j];
				arr[j]=tem;
			}
		}
	}
	return 0;
}

int hotelWrite(){//update hotel room information in hotelSim.txt as final destination when program terminates normally
	FILE *hw;//create file pointer for writing
	hw=fopen(hotelPath,"w");//the path should be same as the source path of hotel text file used at the beginning of the program
	if(hw==NULL){//call error printing message if file not found, which should not appear normally
		perror("Error in opening hotel information file\n");
		system("pause");
	}

	fprintf(hw,"%d#",h.totalRoom);//write hotel's total rooms number
	fprintf(hw,"%d#",h.totalStandard);//write hotel's total standard rooms number
	fprintf(hw,"%d#",h.totalLuxury);//write hotel's total luxury rooms number
	fprintf(hw,"%d#",h.roomAvailable);//write hotel's total number of rooms available
	fprintf(hw,"%d#",h.standardAvailable);//write hotel's total number of standard rooms available
	fprintf(hw,"%d\n",h.luxuryAvailable);//write hotel's total number of luxury rooms available
	sortArray(h.roomNumbers,h.totalRoom);//sort arrays
	sortArray(h.standardRoomNumbers,h.totalStandard);
	sortArray(h.luxuryRoomNumbers,h.totalLuxury);
	sortArray(h.availableRooms,h.roomAvailable);
	sortArray(h.availableStandard,h.standardAvailable);
	sortArray(h.availableLuxury,h.luxuryAvailable);
	sortArray(h.occupied,(h.totalRoom-h.roomAvailable));

	//if room numbers are greater than 24, write the room numbers in two lines in the text file
	if(h.totalRoom<=24){//if room numbers smaller than or equal to 24, write as normal
		for(int a=0;a<h.totalRoom;a++){//loop a times, a refers to the total number of rooms in hotel
			fprintf(hw,"%d#",h.roomNumbers[a]);//write every single room numbers in the hotel without considering its type and availability, separated by a comma
		}
		fprintf(hw,"\n");//write next part after a new line
	}
	else{//if greater than 24, write in two lines
		for(int j=0;j<24;j++){
			fprintf(hw,"%d#",h.roomNumbers[j]);
		}
		fprintf(hw,"\n");
		for(int k=24;k<h.totalRoom;k++){
			fprintf(hw,"%d#",h.roomNumbers[k]);
		}
		fprintf(hw,"\n");
	}


	for(int e=0;e<h.totalStandard;e++){//loop e times, e refers to the total number of standard rooms
		fprintf(hw,"%d#",h.standardRoomNumbers[e]);//write every single room numbers in standard room type without considering its availability, separated by a comma
	}
	fprintf(hw,"\n");//write next part after a new line

	for(int f=0;f<h.totalLuxury;f++){//loop f times, f refers to the total number of luxury rooms
		fprintf(hw,"%d#",h.luxuryRoomNumbers[f]);//write every single room numbers in luxury room type without considering its availability, separated by a comma
		}
	fprintf(hw,"\n");//write next part after a new line

	for(int b=0;b<h.roomAvailable;b++){//loop b times, b refers to the total number of rooms available
		fprintf(hw,"%d#",h.availableRooms[b]);//write every single room numbers of rooms available without considering its room type, separated by a comma
	}
	fprintf(hw,"\n");//write next part after a new line

	for(int c=0;c<h.standardAvailable;c++){//loop c times, c refers to the total number of standard rooms available
		fprintf(hw,"%d#",h.availableStandard[c]);//write every single room numbers of standard rooms available, separated by a comma
	}
	fprintf(hw,"\n");//write next part after a new line

	for(int d=0;d<h.luxuryAvailable;d++){//loop d times, d refers to the total number of luxury rooms available
		fprintf(hw,"%d#",h.availableLuxury[d]);//write every single room numbers of luxury rooms available, separated by a comma
	}
	fprintf(hw,"\n");//write next part after a new line

	for(int g=0;g<(h.totalRoom-h.roomAvailable);g++){//loop g times, g refers to the total number of rooms occupied
		fprintf(hw,"%d#",h.occupied[g]);//write every single room numbers of rooms occupied without considering its room type, separated by a comma
	}
	fclose(hw);//close file pointer
	return 0;
}

int input_Check(char input[]){//called when program ask for user's confirmation, user should input Y/N to indicate his/her choice, this function validates the input and returns a value to indicate of the input is valid
	if(strcmp("Y",input)==0){//if input is "Y", return 1, indaicating input is valid and user's choice is YES
		return 1;
	}
	else if(strcmp("N",input)==0){//if input is "N", return 0, indicating input is valid and user's choice is NO
		return 0;
	}
	else{//else, which mean user's choice is neither "Y" nor "N", which is an invalid input, return 2 as invalid indication
		return 2;//it will be better to return NULL but in this program NULL is defined as the same value of 0, therefore NULL is not applicable
	}
}

const char* findType(int room){//call to find the room type of a given room number (room number input by the user)
	//local declarations
	char ty[20];//variable storing the type
	bool lux=false;//set variable lux as false for checking the room type in next step
	bool std=false;//set variable std as false for checking the room type in next step

	//search the room number in both arrays storing the room numbers with different types, to guarantee room number is found in the array
	for(int a=0;a<h.totalLuxury;a++){//loop a times, a refers to the total number of luxury rooms in the hotel
		if(room==(h.luxuryRoomNumbers[a])){//check every single luxury room numbers in the array, if room number input by the user is the same as anyone in the array, execute this part
			lux=true;//set lux to true to indicate that the room number is found in the array storing all luxury rooms, which means the room number is a luxury room
		}
	}
	for(int b=0;b<h.totalStandard;b++){//loop b times, b refers to the total number of standard rooms in the hotel
		if(room==(h.standardRoomNumbers[b])){//check every single standard room numbers in the array, if room number input by the user is the same as anyone in the array, execute this part
			std=true;//set std to true to indicate that the room number is found in the array storing all standard rooms, which means the room number is a standard room
		}
	}

	//save the result in ty for returning according to the searching result
	if(lux==true){//if it is found that it is a luxury room, execute this part
		strcpy(ty,"Luxury");//copy type "Luxury" into ty for returning
	}
	else if(std==true){//if it is found that it is a standard room, execute this part
		strcpy(ty,"Standard");//copy type "Standard" into ty for returning
	}
	else{//normally will not appear in this program: if both arrays cannot find the room number, execute this part
		strcpy(ty,"ERROR");//print an error message
	}
	return ty;//return the found room type
}

const char* findBed(int room){//call to find the bed type of a given room number (room number input by the user)
	//local declarations
	char bed[20];//variable storing the bed type

	//according to the assumptions, all even room numbers are with a twin bed
	//find if it is an even number to determine its bed type
	if(room%2==0){//if room number is an even number, execute this part
		strcpy(bed,"Twin");//copy bed type "Twin" into bed for returning
	}
	else if(room%2==1){//if room number if an add number, execute this part
		strcpy(bed,"Two Single");//copy bed type "Two Single" into bed for returning
	}
	return bed;//return the found bed type
}

int roomInfo(int rn){//call to display the details of a room of a given room number (room number input by the user)
	//local declarations
	bool exist=false;//set variable exist to false for checking if the room exist in the next step
	
	//check if a room is a valid room number
	if(rn>=101){//if room number is valid (it is a number and greater than 100), execute this part
		//search the array and check if the room exist or not
		for(int m=0;m<h.totalRoom;m++){//loop m times, m refers to the total number of rooms in the hotel
			if(h.roomNumbers[m]==rn){//search in the array contains all room numbers, check if anyone room number in the array matches the given room number
				exist=true;//if any match case found, set exist to true to indicate the room exist
			}
		}
		
		if(exist==true){//if room exist, execute this part
			//another local declarations
			char occ[100];//variable storing the status of the room (occupied or available)
			char typ[20];//variable storing the room type
			char bed[20];//variable storing the bed type
			int r;//variable used in for loop

			//display the room details
			printf("\nRoom No.:  %d\n",rn);//print the room number
			strcpy(typ,findType(rn));//call findType to find the room type, then store the returning result in typ
			strcpy(bed,findBed(rn));//call findBed to find the bed type, then store the returning result in bed
			printf("Room type: %s\n",typ);//print the room type
			printf("Bed type:  %s\n",bed);//print the bed type
			for(r=0;r<h.roomAvailable;r++){//loop r times, r refers to the total number of rooms available
				if(h.availableRooms[r]==rn){//search every single room number in the arrays containing all available room numbers, if any match case found, execute this part
					strcpy(occ,"Available\0");//if match case is found, copy "Available" to the variable occ, indicating the status of the room is available
				}
			}
			if(strcmp(occ,"Available\0")!=0){//after searching, if the variable storing the status of the room is not "Available", execute this part
				strcpy(occ,"Occupied\0");//set the variable storing the status of the room as "Occupied"
			}
			printf("Status:    %s\n\n",occ);//print the status of the room
		}
		else{//if variable exist is still false (room number cannot found in the room numbers array), execute this part
			printf("Room %d not exist!\n",rn);//print error message, indicating the room does not exist
		}
	}
	else{//if room number is found invalid, execute this part
		printf("Invalid room number. Heading back to menu\n");//print error message and get back to menu page
	}
	printf("\nPress any key to get back to menu.\n\n");//works as a buffer for user to see the message instead of getting back to the menu page immediately
	getch();
	return 0;
}

int hotelInput(){//called at the beginning of the program, to load the updated details of hotel rooms into the program before any operations
	//local declarations
	FILE *hp;//create file pointer
	char row[100];//variable storing every line read from the text file
	char *tok;//point to every token of data broken down from a row after each strtok operation
	int count=0;//local counting variable, counting the current line number when reading from text file
	bool nex=false;

	//open the file
	hp=fopen(hotelPath,"r");//open the file for reading, the source path should be same as the one used at the end of the program
	if(hp==NULL){//if error occurred in file opening, execute this part, normally will not run this part in this program
		perror("Error in opening hotel information file, please set the file opening path before execution\n");//print error message
		system("pause");
	}

	//read from the file and load into the program by storing the data in different variables
	while(fgets(row,sizeof(row),hp)!=NULL){//use while loop to read every single line in the text file
		//another local declarations
		int m=0;//counting the number of tokens to indicate the correct position on data reading
		int b=0;//variables b to i are used for counting the position in the arrays and used during different lines in the text file,
		int c=0;//Besides the first line, the other lines are representing different arrays containing different room numbers with 
		int d=0;//different categorized features, like availability and room types.
		int e=0;
		int f=0;
		int g=0;
		int i=0;
		if(count==0){//if it is the first line in the text file, execute this part
			tok=strtok(row,"#");//break the line of data into tokens, separated by "#" (the reasons of using "#" in the text files will be explained in the report)
			while(tok!=NULL){//while the end of the line has not reached
				if(m==0){//first token
					h.totalRoom=atoi(tok);//convert into integer value and store as the total room numers of the hotel
				}
				if(m==1){//second token
					h.totalStandard=atoi(tok);//convert to integer value and store as the total standard room numbers of the hotel
				}
				if(m==2){//third token
					h.totalLuxury=atoi(tok);//convert to integer value and store as the total luxury room numbers of the hotel
				}
				if(m==3){//forth token
					h.roomAvailable=atoi(tok);//convert to integer value and store as the number of rooms available of the hotel
				}
				if(m==4){//fifth token
					h.standardAvailable=atoi(tok);//convert to integer value and store as the number of standard rooms available of the hotel
				}
				if(m==5){//sixth token
					h.luxuryAvailable=atoi(tok);//convert to integer value and store as the number of luxury rooms available of the hotel
				}
				tok=strtok(NULL,"#");//perform strtok operation to find the next token
				m++;//increment on m, indicating the position of the token (first/second/...)
			}
		}
		else{
			tok=strtok(row,"#");//break down the current line into tokens separated by commas
			while(tok!=NULL){//while the current line has not reached the end
				if(count==1){//2nd line of the text file
					if(h.totalRoom<=24){
						h.roomNumbers[b]=atoi(tok);//convert every single token to integer each time and store into the array storing all the room numbers
						b++;//increment on b, indicating the position of the token (first/second/...) and indicating the position in the array (0/1/2/...)
					}
					else{
						if(b<24){
							h.roomNumbers[b]=atoi(tok);//convert every single token to integer each time and store into the array storing all the room numbers
							b++;//increment on b, indicating the position of the token (first/second/...) and indicating the position in the array (0/1/2/...)
						}
						else{
							nex=true;
						}
					}
				}
				else if((count==2)&&(h.totalRoom>24)&&(nex==true)){
					h.roomNumbers[b+24]=atoi(tok);
					b++;
				}
				else if(count==3){//3rd line of the text file
					h.standardRoomNumbers[c]=atoi(tok);//convert every single token to integer each time and store into the array storing all the standard room numbers
					c++;//increment on c, indicating the position of the token (first/second/...) and indicating the position in the array (0/1/2/...)
				}
				else if(count==4){//4th line of the text file
					h.luxuryRoomNumbers[d]=atoi(tok);//convert every single token to integer each time and store into the array storing all the luxury room numbers
					d++;//increment on d, indicating the position of the token (first/second/...) and indicating the position in the array (0/1/2/...)
				}
				else if(count==5){//5th line of the text file
					h.availableRooms[e]=atoi(tok);//convert every single token to integer each time and store into the array storing all the available room numbers
					e++;//increment on e, indicating the position of the token (first/second/...) and indicating the position in the array (0/1/2/...)
				}
				else if(count==6){//6th line of the text file
					h.availableStandard[f]=atoi(tok);//convert every single token to integer each time and store into the array storing all the available standard room numbers
					f++;//increment on f, indicating the position of the token (first/second/...) and indicating the position in the array (0/1/2/...)
				}
				else if(count==7){//7th line of the text file
					h.availableLuxury[g]=atoi(tok);//convert every single token to integer each time and store into the array storing all the available luxury room numbers
					g++;//increment on g, indicating the position of the token (first/second/...) and indicating the position in the array (0/1/2/...)
				}
				else if(count==8){//8th line of the text file
					h.occupied[i]=atoi(tok);//convert every single token to integer each time and store into the array storing all the occupied room numbers
					i++;//increment on i, indicating the position of the token (first/second/...) and indicating the position in the array (0/1/2/...)
				}
				tok=strtok(NULL,"#");//perform strtok operation to find the next token
			}
		}
		count++;//increment on count, indicating going to the next line of the text file
	}
	fclose(hp);//close file pointer
	return 0;
}

int customerInput(){//called at the beginning of the program, right after hotelInput(), to load the updated customers' information into the program before any operations
	//local declarations
	FILE *fp;//create file pointer
	char line[100];//variable storing every line read from the text file
	char *split;//point to every token of data broken down from a line after each strtok operation
	int count=0;//local counting variable, counting the current line number when reading from text file, which also represents the index of the customer number (0=1st customer/1=2nd/...)
	
	//open the file
	fp=fopen(currentPath,"r");//file source path should be same as the one used at the end of the program in customerWrite()
	if(fp==NULL){//if error occurred in file opening, execute this part, normally will not appear in this program
		perror("Error opening file, please set the file opening path before execution\n");//print error message
		system("pause");
	}

	//read from the file and load the data into the program and store them into different variables
	while(fgets(line,sizeof(line),fp)!=NULL){//use while loop to read every single line in the text file
		//another local declarations
		int x=0;//variable used to count the position of tokens in the current line
		split=strtok(line,"#");//break the line of data into tokens, separated by "#" (the reasons of using "#" in the text files will be explained in the report)
		while(split!=NULL){//while the end of the line has not reached
			if(x==0){//1st token
				strcpy(c[count].name,split);//copy the current token to the name of current index (count) of customer
			}
			if(x==1){//2nd token
				strcpy(c[count].address,split);//copy the current token to the address of current index (count) of customer
			}
			if(x==2){//3rd token
				strcpy(c[count].id,split);//copy the current token to the id/passport of current index (count) of customer
			}
			if(x==3){//4th token
				strcpy(c[count].date,split);//copy the current token to the check-in date of current index (count) of customer
			}
			if(x==4){//5th token
				strcpy(c[count].room.number,split);//copy the current token to the room number of current index (count) of customer
			}
			if(x==5){//6th token
				c[count].room.charges=atof(split);//copy the current token to the room charges of current index (count) of customer
			}
			if(x==6){//7th token
				c[count].room.fee=atof(split);//copy the current token to the room fee of current index (count) of customer, optional as its value is always zero
				strcpy((c[count].room.type),(findType(atoi(c[count].room.number))));//call findType function with room number to find the room type and copy the result to the room type of current index (count) of customer
				strcpy(c[count].room.bed,findBed(atoi(c[count].room.number)));//call findBed function with room number to find the bed type and copy the result to the bed type of current index (count) of customer
				count++;//increment on count to indicate its jumping to the next customer (increment on customer index)
			}
			split=strtok(NULL,"#");//perform strtok operation to find the next token
			x++;//increment on x, indicating the position of the tokens (first/second/...)
		}
	}
	fclose(fp);//close file pointer
	return count;//return after all date of customers in text file are loaded, the return value will be the current number of customer in the hotel
}

int removeRoomNum(int room[100], int total, int rmNum){//this function is used to remove a specified room number (rmNum), from a given array (room[100]), given the total number of items in that array (total)
	//local declarations
	int c, r;//variables used for counting in the next step

	//find and remove
    for ( c = 0 ; c < total ; c++ ){//loop c times, c refers to the number of items in array room[100] (the maximum room number ([100]) can be modified)
		if(rmNum==room[c]){//search the position of the required room number in the given array
			for(r=c;r<total;r++){//if its position is found, loop r times, r refers to the number of remaining items in the array after the required room number (if the required room number is at index 3, while the total items in the array is 10, r will loop 7 times)
				room[r]=room[r+1];//to remove the required room number, set the item after the required room number as the item in current index until the second last item is reached
				if(r==(total-r-1)){//if second last item is reached
					room[r+1]=NULL;//set last item as NULL
				}
			}
		}
	}
	return 0;
}

int findCustomer(char cu[20]){//called when the index of the customer need to be find before doing any operations
	//local declarations
	FILE *fp;//create file pointer
	char line[100];//variable storing every line read from the text file
	char *split;//point to every token of data broken down from a line after each strtok operation
	int x=0;//local counting variable to count the line number of the current customer , which is also the customer index
	char name[20];//variable storing the token of the name of the customer at every line
	char room[5];//variable storing the token of the room number of the customer at every line
	bool check=false;//variable used to check if the customer can be found in the text file in next step
	bool chec=false;//another variable used to double check if the customer has a record in the hotel
	
	//open the file
	fp=fopen(currentPath,"r");//open the file using the current path, which should be the file location of where the customer text file stored
	if(fp==NULL){//if error iccurred in file opening, execute this part, normally should not appear in this program
		perror("Error opening file\n");//print error message
		system("pause");
	}

	//read from the text file
	while(fgets(line,sizeof(line),fp)!=NULL){//use while loop to read every single line in the text file
		split=strtok(line,"#");//break the line of data into tokens, separated by "#" (the reasons of using "#" in the text files will be explained in the report)
		strcpy(name,split);//copy the customer name to variable name
		if(strcmp(name,cu)==0){//check if customer name at current line matches the input customer name
			strcpy(room,c[x].room.number);//if matches, copy customer room number to room
			check=true;//set variable check to true
			break;
		}
		x++;//increment on x, indicating going to next line and look for next name for checking
	}

	//after searching all the names tokens in the text file
	if(check==false){//cannot find customer matches the name
		for(int r=0;r<customerCount;r++){//loop r times, r refers to the updated number of customers in hotel
			if(strcmp(c[r].name,cu)==0){//check if customer may be missed in the checking from text file, check again using program's current data of customers
				x=r;//if customer can be found, set x to the new updated customer index
				chec=true;//set chec to true
			}
		}
		if(chec==false){//if chec is still false (customer does not have a record in the hotel), execute this part
			x=9999;//set the return value x as 9999 to indicate customer does not have a record in the hotel (cannot use NULL as it has another value 0, which is a valid index number)
		}
	}
	fclose(fp);//close file pointer
	return x;//return either the customer index if customer is found or return 9999 as an indicator of non-existing customer
}

int customerSort(int count){//called when displaying customer information is required, this function sorts the customer list by the alphabetic order in their names
	//local declarations
	struct Copy{//set a structure for storing a cpoy of names for sorting but won't affect the record in the program
		char name[20];
	}copy[100];
	int r;//variable used in the for loop in next step
	int order[100];//an integer array for storing the ordered indexes after sorting

	//copying all customer names into structure copy for sorting
	for(r=0;r<count;r++){//loop r times, r refers to the number of customers currently in the hotel, which is given as parameter when function is called
		strcpy(copy[r].name,c[r].name);//copy the name one by one with correct index
		order[r]=r;//insert the index according to the initial order of the names
	}

	//sorting
	char cpy[20];//variable for storing the customer names temporarily
	int a,y,z;//variables used in the for loops in next steps
	//loop y*z times in total to make sure each names compare with every other names at least once to perform sorting
	for(y=1;y<count;y++){//loop y times, y refers to the number of customers
		for(z=1;z<count;z++){//loop z times, z refers to the number of customers
			if(strcmp(copy[z-1].name,copy[z].name)>0){//if the next name has a higher alphabetic order than the current one, swap the name and order
				strcpy(cpy,copy[z-1].name);//swap the two names with temp variable cpy
				strcpy(copy[z-1].name,copy[z].name);
				strcpy(copy[z].name,cpy);
				int temp;//swap the index in order[] too with a temp variable for storing the sorted result
				temp=order[z-1];
				order[z-1]=order[z];
				order[z]=temp;
			}
		}	
	}

	//print out the sorted result
	printf("\n");
	for(a=0;a<count;){//loop a times, a refers to the number of customers
		if(strcmp(c[order[a]].name,"")!=0){//if the last custoer has not reached, execute this part
			printf("Customer Record #%d:\n\n",(a+1));//print customer record with corresponding index from order[] (sorted order index)
			printf("Name:          %s\n",c[order[a]].name);//print customer name
			printf("Address:       %s\n",c[order[a]].address);//print customer address
			printf("ID/passport:   %s\n",c[order[a]].id);//print customer id/passport
			printf("Check-in date: %s\n",c[order[a]].date);//print customer check-in date
			printf("Room number:   %s\n",c[order[a]].room.number);//print customer room number
			printf("Room type:     %s\n",c[order[a]].room.type);//print customer room type
			printf("Bed type:      %s\n\n\n",c[order[a]].room.bed);//print customer bed type
			a++;//increment on a, for printing next customer
		}
	}
	return 0;
}

int customers(int count){//called when display of customers' information is required
	printf("\n%s\n\n",stars);//formatting display
	printf("Listing customer according to alphabetic order on customer names:\n");//show user that the customer list is listed according to the alphabetic order on the names
	customerSort(count);//sort the customer list and print the result
	printf("\n%s\n",stars);//formatting display
	printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
	getch();
	return 0;
}

int hotelInfo(){//called when display of hotel's information is required
	printf("\n%s\n\n",stars);//formatting display
	printf("Hotel name: %s\n",h.name);//print hotel's name
	printf("Hotel address: %s\n",h.address);//print hotel's address
	printf("Hotel rooms in total:    %d  (Standard: %2d  Luxury: %2d)\n",h.totalRoom,h.totalStandard,h.totalLuxury);//print hotel's statistics on total rooms
	printf("Hotel room(s) available: %d  (Standard: %2d  Luxury: %2d)\n",h.roomAvailable,h.standardAvailable,h.luxuryAvailable);//print hotel's statistics on rooms available
	printf("Hotel restaurant: %s\n",h.restaurant);//print hotel's restaurant name
	printf("\n%s\n",stars);//formatting display
	printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
	getch();
	return 0;
}

int findRoom(char cu[20]){//this function is used to find the room number of the specified customer (name entered by user)
	int d=findCustomer(cu);//call findCustomer() to find the customer index
	if(d!=9999){//if the function can find customer, execute this part
		printf("\nCustomer: %s\nRoom number: %s\n",c[d].name,c[d].room.number);//print room number of where the customer in according to the customer index
	}
	else{//if customer cannot be found, execute this part
		printf("Customer not exist.\n");//print error message telling the user that the customer does not exist (no corresponding customer record in the data)
	}
	printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
	getch();
	return 0;
}

int findOccupier(int n){//this function is used to find the room occupier given a specified room number (roomn number entered by user)
	//local declarations
	char occupier[20];//variable storing the occupier's name for printing
	int r, i, k;//variables used in the for loops in next steps
	int host=NULL;//variable used to check if that room is available or occupied
	bool exis=false;//variable used to check if the room exist

	//search and check if the room number entered by the user is valid and exist
	for(k=0;k<h.totalRoom;k++){//loop k times, k refers to the number of total rooms in the hotel
		if(h.roomNumbers[k]==n){//if any room numbers i9n the array containing all room numbers matches the input room numbers, execute this part
			exis=true;//set exis to true to indicate the room exist (valid room numer)
		}
	}

	//find occupier
	if((n>=101)&&(n<=(100+h.totalRoom))&&(exis==true)){//if room number is valid and the room exist, execute this part
		for(i=0;i<h.roomAvailable;i++){//loop i times, i refers to the number of rooms available in the hotel
			if(h.availableRooms[i]==n){//if room number matches any room number in the array containing all available room numbers, execute this part
				host=1;//set host to 1 to indicate room is available, which has no occupier currently
			}
		}
		
		//after searching in rooms available array
		if(host==1){//if host equals 1, execute this part
			printf("\nRoom %d is not occupied.\n",n);//print result message
		}
		else{//room is occupied
			for(r=0;r<customerCount;r++){//loop r times, r refers to the number of customers currently in the hotel
				if(n==atoi(c[r].room.number)){//if room number matches any customer's room number, execute this part
					strcpy(occupier,c[r].name);//store the result (the corresponding customer's name) according to the found customer index
				}
			}
			printf("\nRoom number: %d\nOccupier: %s\n",n,occupier);//print the occupier together with the room number
		}
	}
	else{//if room number is invalid or not exist, execute this part
		printf("Invalid room number. (Room not exist!)\n");//print error message
	}
	printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
	getch();
	return 0;
}

int newRoom(int roomnumber, char type[10]){//insert new room numbers into the appropriate arrays
	if(strcmp(type,"Standard")==0){//if room type is standard, execute this part
		h.standardAvailable+=1;//update the number of standard available rooms
		h.availableStandard[h.standardAvailable-1]=roomnumber;//insert the new room number into the array containing standard available room numbers
		h.totalStandard+=1;//update the number of total standard rooms
		h.standardRoomNumbers[h.totalStandard-1]=roomnumber;//insert the new room number into the array containing all standard room numbers
	}
	else{//if room type is not standard (luxury), execute this part
		h.luxuryAvailable+=1;//update the number of standardluxury available rooms
		h.availableLuxury[h.luxuryAvailable-1]=roomnumber;
		h.totalLuxury+=1;//update the number of total luxury rooms
		h.luxuryRoomNumbers[h.totalLuxury-1]=roomnumber;//insert the new room number into the array containing all luxury room numbers
	}
	return 0;
}

int buildNewRoom(char type[10]){//called when user (admin) wants to build a new room in the hotel due to expansion
	if((strcmp(type,"Standard")==0)||(strcmp(type,"Luxury")==0)){//if the input type is valid
		//local declarations
		char confirm[20];//storing input for confirmation
		int newRoomNumber=0;

		//ask for user confirmation
		printf("You are going to build a room, room type: %s\n",type);//ask if user confirm to build the room with specified room type
		printf("Confirm? Y/N\n");
		gets(confirm);//gets user input
		if(input_Check(confirm)==1){//if input validation returns 1 ("Y"), execute this part
			//another local declarations
			int roomCpy[100];

			h.totalRoom+=1;//update total number of rooms of the hotel
			h.roomAvailable+=1;//update the number of rooms available of the hotel

			//find new room number
			for(int t=0;t<(h.totalRoom-1);t++){//insert all room numbers into roomCpy for sorting
				roomCpy[t]=h.roomNumbers[t];
			}

			//sort the roomCpy array for checking any skipped room numbers
			sortArray(roomCpy,(h.totalRoom-1));

			int q=0;//variable for counting the existing room numbers
			while((roomCpy[q+1]==(roomCpy[q]+1))&&(q<h.totalRoom-1)){//find if there is room number skipped due to previous removal
				q++;//if room number does not skip, increment on q
			}//while loop stop when a number is skipped or all room number did not skip
			newRoomNumber=q+102;//assign the new room number

			//update the arrays with newRoomNumber
			h.availableRooms[h.roomAvailable-1]=newRoomNumber;//insert the new room number into array containing all available room numbers
			h.roomNumbers[h.totalRoom-1]=newRoomNumber;//insert the new room number into array containing all room numbers
			newRoom(newRoomNumber,type);//update room available arrays of corresponding room type
			//hotelWrite();//optional, may or may not update the hotel text file right after building the room
			printf("You have successfully build a new room - room number: %d.\n",newRoomNumber);//print result message
			printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
			getch();
		}
		else if(input_Check(confirm)==0){//if input validation returns 0 ("N"), execute this part
			printf("You have cancelled to build a new room.\n");//print message tell user that he/she has cancelledto build a new room
			printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
			getch();
		}
		else if(input_Check(confirm)==2){//if input validation returns 2 (neither "Y" nor "N"), execute this part
			printf("Wrong input please enter again!\n");//print error message on invalid input
			buildNewRoom(type);//recall the function
		}
	}
	else{//invalid input type
		printf("\nInvalid input. Heading back to menu\n");//print error message
	}
	return 0;
}

int removeRoom(int n){//function to remove a particular room in the hotel due to renovation
	if((n>=101)&&(n<=(100+h.totalRoom))){//if room number input is valid, execute this part
		//local declarations
		char confirm[20];//variable storing input for confirmation
		bool own=false;//variable used to check if the room is currently occupied by a customer

		//search the array containing all occupied room numbers
		for(int v=0;v<(h.totalRoom-h.roomAvailable);v++){//loop v times, v refers to the number of occupied rooms
			if(h.occupied[v]==n){//if room number is found in the array, execute this part
				own=true;//set own to true to indicate the room is occupied
			}
		}

		if(own==true){//if the room is currently occupied, execute this part
			printf("\nRoom %d is now occupied!\n",n);//print error message that the room cannot be removed as it is occupied
			printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
			getch();
		}
		else{//if the room is not occupied by any customers, execute this part
			printf("Remove room: %d\nConfirm? Y/N\n",n);//ask for user confirmation on removal of the particular room
			gets(confirm);//get user input
			if(input_Check(confirm)==1){//if user input is "Y", execute this part
				//another local declarations
				bool find=false;//variable used to check if that room number can be found in the array of standard type available room numbers
				
				for(int g=0;g<h.standardAvailable;g++){//loop g times, g refers to the number of standard available rooms
					if(h.availableStandard[g]==n){//if room number can be found in this array, execute this part
						find=true;//set find to true to indicate the room number is in standard type and not occupied now
					}
				}

				if(find==true){//if the room is not occupied and in standard type, execute this part
					removeRoomNum(h.availableStandard,h.standardAvailable,n);//call function to remove the room number in the array containing standard available room numbers
					h.standardAvailable-=1;//decrement the number of standard availble rooms
					removeRoomNum(h.standardRoomNumbers,h.totalStandard,n);//call function to remove the room number in the array containing all standard room numbers
					h.totalStandard-=1;//decrement the number of total standard rooms of the hotel
				}	
				else{//if find is not true, which means the room is not occupied and in luxury type, execute this part
					removeRoomNum(h.availableLuxury,h.luxuryAvailable,n);//call function to remove the room number in the array containing luxury available room numbers
					h.luxuryAvailable-=1;//decrement the number of luxury availble rooms
					removeRoomNum(h.luxuryRoomNumbers,h.totalLuxury,n);//call function to remove the room number in the array containing all luxury room numbers
					h.totalLuxury-=1;//decrement the number of total luxury rooms of the hotel
				}

				//update the arrays with n (the room number removed)
				removeRoomNum(h.availableRooms,h.roomAvailable,n);//remove the room number in the array containing all available room numbers
				removeRoomNum(h.roomNumbers,h.totalRoom,n);//remove the room number in the array containing all the room numbers
				h.totalRoom-=1;//decrement the total room numbers
				h.roomAvailable-=1;//decrement the number of available rooms
				printf("You have remove room %d successfully.\n",n);//print result message
				printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
				getch();
			}
			else if(input_Check(confirm)==0){//if user input on confirmation is "N", execute this part
				printf("You have cancelled to remove the room.\n");//print message telling user had just cancelled to remove a room
				printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
				getch();
			}
			else if(input_Check(confirm)==2){//if user input on confirmation is neither "Y" nor "N", execute this part
				printf("Invalid input. Please enter again!\n");//print error message indicating invalid input
				removeRoom(n);//recall function
			}
		}
	}
	else{//if room number input is invalid, execute this part
		printf("Invalid room number.\n");//print error message
		printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	return 0;
}

int rand_Room(int list[100],int total, char bed[10]){//function to generate a random room number from the specified rooms available array according to the bed type
	//local declarations
	srand(time(0));//for using rand()
	int num=0;//cariable storing the returned room number generated
	bool ok=false;//variable used for checking if room number generated fits the requirements

	//according to the assumptions, only even room numbers are with Twin bed
	if(strcmp(bed,"Twin")==0){//if the bed type chosen is "Twin", execute this part
		while(ok==false){//loop until the randomnly generated room number fits the requirement
			num=list[rand()%(total)];//generate a random room number from the particular array given in parameters
			if(num%2==0){//if room number generated is even, execute this part
				ok=true;//set ok to true to indicate the requirement is fit now
				return num;//return the checked room number
			}
		}
	}

	else if(strcmp(bed,"Two Single")==0){//if the bed type chosen is "Two Single", execute this part
		while(ok==false){//loop until the randomnly generated room number fits the requirement
			num=list[rand()%(total)];//generate a random room number from the particular array given in parameters
			if(num%2!=0){//if room number generated is odd, execute this part
				ok=true;//set ok to true to indicate the requirement is fit now
				return num;//return the checked room number
			}
		}
	}
}

int enter_club(char cu[20], int n, int w){//called when user choose/pick the club entrance service, available only to the customers in luxury rooms
	//local declarations
	float entranceCost=50;//variable storing the entrance cost of the club
	char confirm[20];//variable storing input for confirmation
	int rmn=0;//variable storing the room number
	rmn=atoi(c[w].room.number);//store the integer form of room number of the customer with corresponding customer index given in parameters

	//if customer has a record in hotel (checked-in) and the room number is input correctly, execute this part
	if((w!=9999)&&(n==rmn)){//(w!=9999) means the customer has checked-in
		if(strcmp(c[w].room.type,"Luxury")==0){//if customer is in luxury room, execute this part
			//print the club entrance cost and details on charges and ask for user's confirmation on permission of club entrance
			printf("\nYou are going to enter the club with an entrance fee costs HK$%.2f per visit.\n",entranceCost);
			printf("Fees will be charged to room %s\n",c[w].room.number);
			printf("Confirm? Y/N\n");
			gets(confirm);//get user's input

			if(input_Check(confirm)==1){//if user's input is "Y", execute this part
				c[w].room.charges+=entranceCost;//charge the entrance fee to the customer's room (accumulated)
				printf("You have entered the club successfully.\n",n);//print result message
				printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
				getch();
			}
			else if(input_Check(confirm)==0){//if user's input is "N", execute this part
				printf("You have cancelled to enter the club.\n");//print message indicating that user had just cancelled to enter the club
				printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
				getch();
			}
			else if(input_Check(confirm)==2){//if user's input is neither "Y" nor "N", execute this part
				printf("Wrong input please enter again!\n");//print error message showing that user's input is invalid
				enter_club(cu,n,w);//recall function
			}
		}
		else{//if customer is not in luxury room (standard), execute this part
			printf("\nClub entrance is denied.\nOnly members in luxury room can enter the club.\n");//print denying message
			printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
			getch();
		}
	}
	else if(w==9999){//if customer does not has a record in hotel system, execute this part
		printf("\nCustomer has not checked-in.\nHeading back to menu\n");//print error message as customer had not checked-in
	}
	else if(n!=rmn){//if customer has checked-in but input a wrong or an invalid room number, execute this part
		printf("\nClub entrance is denied due to invalid membership number.\n");//print denying message due to invalid club membership number
		printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	else{//if input is invalid (e.g. not a number), execute this part
		printf("\nInvalid input. Heading back to menu\n");//print error message
	}
	return 0;
}

int drink(char cu[20], int n,int w){//called when user choose to order bottle(s) of wine in cocktail bar, available only to the customers in luxury rooms
	//local declarations
	//wine costs HK$90 each
	float wineCost=90;//variable storing the cost of each bottle of wine
	float totalCost=0;//variable storing the total cost on ordering bottle(s) of wine
	char confirm[20];//variable storing input for confirmation
	char room[20];//variable storing club membership number

	//check club membership number
	printf("Enter customer's club membership number: (same as room nunber)\n");
	gets(room);

	//assume the maximum ordering amount is 10 bottles of wine each time
	//if customer has a record in hotel (checked-in) and the order amount is valid (1-10), execute this part
	if((w!=9999)&&(n>0&&n<=10)&&(atoi(c[w].room.number)==(atoi(room)))){//assume customer can buy at most 10 bottles of wine at a time
		if(strcmp(c[w].room.type,"Luxury")==0){//if customer is in luxury room, execute this part
			//print the total cost on wine ordering and details on charges and ask for user's confirmation on wine ordering
			totalCost=n*wineCost;//calculate the correct total cost on wine ordering
			printf("\nNumber of wine to be ordered: %d\nTotal cost: HK$%.2f (HK$%.2f each)\n",n,totalCost,wineCost);
			printf("Fees will be charged to room %s\n",c[w].room.number);
			printf("Confirm? Y/N\n");
			gets(confirm);//get user's input

			if(input_Check(confirm)==1){//if user's input is "Y", execute this part
				c[w].room.charges+=totalCost;//charge the wine ordering cost to the customer's room (accumulated)
				printf("You have ordered %d bottle(s) of wine successfully.\n",n);//print result message
				printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
				getch();
			}
			else if(input_Check(confirm)==0){//if user's input is "N", execute this part
				printf("You have cancelled to order wine.\n");//print message indicating that user had just cancelled to order wine
				printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
				getch();
			}
			else if(input_Check(confirm)==2){//if user's input is neither "Y" nor "N", execute this part
				printf("Wrong input please enter again!\n");//print error message showing that user's input is invalid
				drink(cu,n,w);//recall function
			}
		}
		else{//if customer is not in luxury room (standard), execute this part
			printf("\nCocktail bar is not available in customer's room.\n");//print denying message
			printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
			getch();
		}
	}
	else if(w==9999){//if customer does not has a record in hotel system, execute this part
		printf("\nCustomer has not checked-in.\nHeading back to menu\n");//print error message as customer had not checked-in
	}
	else if(n<=0){//if the ordering amount is less than or equal to zero, execute this part
		printf("\nInvalid number.\nHeading back to menu\n");//print error message
	}
	else if(atoi(c[w].room.number)!=atoi(room)){//if the club membership number is invalid, execute this part
		printf("\nInvalid club membership number.\nHeading back to menu\n");//print error message
	}
	else if(n>10){//if the ordering amount is greater than 10, execute this part
		printf("\nMaximum order at a time is 10 bottles of wine.\nHeading back to menu\n");//print error message indicating the user that the maximum order amount is 10 bottles each time
		printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	else{//if input is invalid (e.g. not a number), execute this part
		printf("\nInvalid input. Heading back to menu\n");//print error message
	}
	return 0;
}

int phone_call(char cu[20], int n,int w){//called when user choose to make a phone call, local call is availble in all rooms, long distance call is available only to the customers in luxury rooms
	//local declarations
	float localCost=0.5;//variable storing local phone cost, HK$0.5/minute
	float longdisCost=5;//variable storing long distance phone cost, HK$5/minute
	int choice=0;//variable storing luxury room's customers' choice
	char confirm[20];//variable storing input for confirmation
	float totalCost;//variable storing the total cost on making a phone call
	char gt[5];//variable storing user's input of the choice on phone call type

	//assume the maximum duration on each phone call is 60 minutes
	//if customer has a record in hotel (checked-in) and the duration input is valid (1-60 minute(s)), execute this part
	if((w!=9999)&&(n>0&&n<=60)){//assunmed each phone call maximum duration is 60 minutes
		printf("\n1 - Local         - HK$%.1f/min\n",localCost);//print the cost on making a local phone call
		if(strcmp(c[w].room.type,"Luxury")==0){//if customer is in luxury room, execute this part
			printf("2 - Long distance - HK$%.1f/min\n",longdisCost);//provide also long distance call for customers in luxury rooms
			printf("\nEnter your choice: (1/2)\n");//ask for user's input of the choice on phone call type
			gets(gt);//get user's input
			choice=atoi(gt);//convert user's input to integer format
		}
		else{//if customer is not in luxury room (standard), execute this part
			choice=1;//customer in standard rooms cannot choose the phone call type
		}

		if((choice==1)||(choice==2)){//if input of the choice is valid (1/2), execute this part
			if(choice==1){//if user choose 1
				totalCost=localCost*n;//calculate the total cost on making a local phone call
				printf("\nYou are making a local phone call for %d minutes, total cost: HK$ %.1f\n",n,totalCost);//print the total cost on making the local phone call and ask for user's confirmation on making the phone call
			}
			else if(choice==2){//if user choose 2
				totalCost=longdisCost*n;//calculate the total cost on making a long distance call
				printf("You are making long distance phone call for %d minutes, total cost: HK$ %.1f\n",n,totalCost);//print the total cost on making the long distance phone call and ask for user's confirmation on making the phone call
			}
			printf("Fees will be charged to room %s\n",c[w].room.number);//print and state clearly that the cost on phone call will be charged to the corresponding customer's room
			printf("Confirm? Y/N\n");//ask for user's confirmation
			gets(confirm);//get user's input

			if(input_Check(confirm)==1){//if user's input is "Y", execute this part
				c[w].room.charges+=totalCost;//calculate the correct total cost on making a phone call
				printf("You have made a phone call successfully.\n");//print result message
				printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
				getch();
			}
			else if(input_Check(confirm)==0){//if user's input is "N", execute this part
				printf("\nYou have cancelled to make a phone call.\nHeading back to menu\n");//print message indicating that user had just cancelled to make a phone call
				printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
				getch();
			}
			else if(input_Check(confirm)==2){//if user's input is neither "Y" nor "N", execute this part
				printf("Wrong input please enter again!\n");//print error message showing that user's input is invalid
				phone_call(cu,n,w);//recall function
			}
		}
		else{//if user's input is neither 1 nor 2, execute this part
			printf("Invalid input. Please choose again!\n");//print error message indicating invalid input
			phone_call(cu,n,w);//recall function
		}
	}
	else if(n<=0){//if the duration of the phone call is less than or equal to zero, execute this part
		printf("\nInvalid number.\nHeading back to menu\n");//print error message
	}
	else if(n>60){//if the duration of the phone call is greater than 60, execute this part
		printf("\nMaximum duration of any calls is 60 minutes. Please try again!\n");//print error message indicating the user that the maximum duration on each call is 60 minutes each time
		printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	else if(isdigit(n)==0){//if input is not a number, execute this part
		printf("\nPlease enter a number.\n");//print error message
		printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	else if(w==9999){//if customer does not has a record in hotel system, execute this part
		printf("\nCustomer has not checked-in.\nHeading back to menu\n");//print error message as customer had not checked-in
	}
	else{//if input is invalid, execute this part
		printf("\nInvalid input. Heading back to menu\n");//print error message
	}
	return 0;
}

int dine(char cu[20], int n, int w){//called when user choose to order set meal(s) from hotel restaurant
	//local declarations
	float cost =50;//variable storing the cost per each set-meal
	float total=0;//variable storing the total cost on ordering set-meal(s)
	char confirm[20];//variable storing input for confirmation

	//assume the maximum order amount is 5 each time
	//if customer has a record in hotel (checked-in) and the order amount is valid (1-5), execute this part
	if((w!=9999)&&(n>0&&n<=5)){//assunmed maximum order amount is 5 each time
		//print the total cost on set-meal(s) ordering and details on charges and ask for user's confirmation on set-meal(s) ordering
		total=cost*n;//calculate the total cost on ordering set-meal(s)
		printf("\nNumber of set-meals to be ordered is %d\nTotal amount: HKD$%.2f (HK$%.2f each)\n",n,total,cost);
		printf("Fees will be charged to room %s\n",c[w].room.number);
		printf("Confirm? Y/N\n");
		gets(confirm);//get user's input

		if(input_Check(confirm)==1){//if user's input is "Y", execute this part
			c[w].room.charges+=total;//calculate the correct total cost on set-meal(s) ordering
			printf("\nYou have successfully made an order!\n");//print result message
			printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
			getch();
		}
		else if(input_Check(confirm)==0){//if user's input is "N", execute this part
			printf("You have cancelled the order.\n");//print message indicating that user had just cancelled to order set-meal(s)
			printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
			getch();
		}
		else if(input_Check(confirm)==2){//if user's input is neither "Y" nor "N", execute this part
			printf("Wrong input please enter again!\n");//print error message showing that user's input is invalid
			dine(cu,n,w);//recall function
		}
	}
	else if(n<=0){//if the order amount is less than or equal to zero, execute this part
		printf("Invalid number.\nHeading back to menu\n");//print error message
	}
	else if(n>5){//if the order amount is greater than 5, execute this part
		printf("Maximum order of set-meals is 5. Please take your order again!\n");//print error message indicating the user that the maximum order amount is 5 each time
		printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	else if(isdigit(n)==0){//if input is not a number, execute this part
		printf("Please enter a number.\n");//print error message
		printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	else if(w==9999){//if customer does not has a record in hotel system, execute this part
		printf("Customer has not checked-in.\nHeading back to menu\n");//print error message as customer had not checked-in
	}
	else{//if input is invalid, execute this part
		printf("\nInvalid input. Heading back to menu\n");//print error message
	}
	return 0;
}

int checkDate(char date[10]){//function to validates the date, called when user check-in or check-out
	//local declarations
	char day[10];//variable storing the date, assumed format is "Day xx"
	char buffer[20];//variable used as a buffer in converting an integer to string format
	int ret=1;//variable storing the return value

	//date is validate in comparison on strings containing different day nunmbers, will be explained in the report in details
	for(int x=1;x<=31;x++){//loop x times, x refers to 31, assumed that there are 31 days in a month, as this program do not provide real time counting in days
		strcpy(day,"Day ");//store the string "Day " in variable day
		strcat(day,itoa(x,buffer,10));//append the number x (converted in string format) on variable day
		if(strcmp(day,date)==0){//compare with the input given in parameter to perform input validation
			ret=0;//if valid, set return value as 0
		}
	}
	return ret;//if input is valid, return 0, else return 1
}

int customerRemove(char c[20], char roomNum[5]){//function to remove the customer record of a particular customer
	//local declarations
	FILE *infp, *outfp;//create two file pointers, one for reading, one for writing
	char line[100];//variable storing every line read from the text file
	char *split;//point to every token of data broken down from a line after each strtok operation
	char name[20];//variable storing customer's name
	char cpy[100];//variable storing the copy of each line
	char newLine[100];//variable storing the data of a new line (to be writen in new text file)
	char empty=NULL;//variable storing nothing for replacing the line with removed customer record
	char swap[200];//variable storing the temporarily path
	int len;//variable storing the length of string, used when removing the "\n" ar the end of each line
	int x=1;//variable storing the current line number

	//make the currentPath as the updated text file source
	//swap the currentPath and swapPath
	if(strcmp(currentPath,"C:\\customerSim.txt")==0){
		infp=fopen(currentPath,"r");
		outfp=fopen("C:\\customerSim2.txt","w");
		strcpy(swapPath,"C:\\customerSim2.txt");
		strcpy(swap,currentPath);//swap=sim
		strcpy(currentPath,swapPath);//current=sim2
		strcpy(swapPath,swap);//swapPath=sim
	}
	else{
		infp=fopen(currentPath,"r");
		outfp=fopen("C:\\customerSim.txt","w");
		strcpy(swapPath,"C:\\customerSim.txt");
		strcpy(swap,currentPath);//swap=sim2
		strcpy(currentPath,swapPath);//current=sim
		strcpy(swapPath,swap);//swapPath=sim2
	}

	if((infp==NULL)&&(outfp!=NULL)){//if error occurred in file opening, execute this part
		perror("Error opening file\n");//print error message, normally won't appear in this program
		system("pause");
	}

	while(fgets(line,sizeof(line),infp)!=NULL){//use while loop to read every single line from the text file
		len=strlen(line);//get the length of the current line
		if(line[len-1]=='\n'){//if the end of the line reached, which should be "\n", execute this part
			line[len-1]=0;//remove the "\n" at the end of each line
		}
		strcpy(cpy,line);//copy the line with "\n" removed to variable cpy
		split=strtok(line,"#");//break down the line and get the first token and store it as split
		strcpy(name,split);//copy the string split to variable name
		if(strcmp(name,c)==0){//if name token is same as the customer who are checking-out, execute this part
			fprintf(outfp,"",empty);//replace the original line with the name of that customer (who is checking-out) by an empty line, to remove his/her record
		}
		else{//if name token is not the same as the one required, execute this part
			fprintf(outfp,"%s\n",cpy);//this current line is not the line to remove, copy the original line back to the text file
		}
		x++;//increment on x, indicating the current line number
	}
	fclose(outfp);//close file pointer
	fclose(infp);//close file pointer
	customerCount-=1;//decrement on customerCount to reduce the current number of customer in the hotel by 1
	return 0;
}

int check_out(char cu[20], char date[10],int w,int custCount){//called when user choose to check-out, customer name and check-out date should be given in the parameters when function is called
	//local declarations
	char* outday;//variable storing the integer format of check-out date
	char* inday;//variable storing the integer format of check-in date
	char backupDate[10];//variable storing the copy of check-out date, used when this function is recalled due to invalid input on confirmation
	int rmNum;//variable storing the room number of the room that customer occupied (the customer who is checking-out)
	char confirm[20];//variable storing input for confirmation
	float displayFee=0;//variable storing the extra fee incurred when customer took other services in the hotel
	char na[20];//variable storing the name of that customer, used in printing the result message when customer successfully checked-out
	char tempDate[10];//variable storing the copy of check-in date

	//copying the dates for back up and break them down into tokens to get the number of the day
	strcpy(backupDate,date);//copy the check-out date string into variable backupDate
	strcpy(na,c[w].name);//copy the customer name string to variable na
	outday=strtok(date," ");//break down the date input to get the number of the day
	outday=strtok(NULL," ");//continue break down and get the second token, which is the number of the check-out day in string format
	strcpy(tempDate,c[w].date);//copy the check-in date to variable tempDate
	inday=strtok(tempDate," ");//break down the check-in date to get the number of the day
	inday=strtok(NULL," ");//continue break down and get the second token, which is the number of the check-in day in string format
	
	//if customer has a record in hotel (checked-in) and the check-out day is not earlier than the check-in date, execute this part
	if((w!=9999)&&(atoi(outday)>=atoi(inday))){//customer checked-in and check-out date is valid
		//print the details of the payment before asking for check-out confirmation
		printf("\nCheck-in day:    %s\nCheck-out day:   %s\n",inday,outday);//print the check-in and check-out date
		rmNum=atoi(c[w].room.number);//convert the customer room number to integer format and store in variable rmNum
		printf("Room number:     %s",c[w].room.number);//print the customer room number
		int duration=atoi(outday)-atoi(inday)+1;//calculate the duration upon customer has checked-in
		displayFee+=(duration*(700*1.1));//calculate the rental fee including 10% charges, assumed that the extra service charges do not require 10% charges
		printf("\nDuration:        %d day(s)",duration);//print the duration
		if(c[w].room.charges!=NULL){//if extra service used, execute this part
			displayFee+=c[w].room.charges;//add the extra charges to the displayFee
		}
		printf("\nExtra charges:   HK$%.2f",c[w].room.charges);//print extra charges incurred
		printf("\nRoom rental fee: HK$%.2f (10%% daily charge included)",(duration*(700*1.1)));//print rental fee including 10% service charge
		printf("\nTotal:           HK$%.2f\n\nOnly CASH is acceptable.\n\n",displayFee);//print the total amount (sum of rental fee and extra charges)
		printf("Confirm? Y/N\n");//ask for user confirmation on check-out
		gets(confirm);//get user's input

		if(input_Check(confirm)==1){//if user's input is "Y", execute this part
			//user confirms to check-out, update the hotel and customer information
			c[w].room.fee=displayFee;//optional as customer's record will be deleted right away
			h.roomAvailable+=1;//increment on number of rooms available of the hotel
			h.availableRooms[h.roomAvailable-1]=rmNum;//add back the room number (of the customer just checked-out) to the array containing all available room numbers
			removeRoomNum(h.occupied,(h.totalRoom-h.roomAvailable+1),rmNum);//call function to remove the room number (of the customer just checked-out) from the array containing all occupied room numbers
			
			if(strcmp(c[w].room.type,"Luxury")==0){//if the room (of the customer just checked-out) is in luxury type, execute this part
				h.luxuryAvailable+=1;//increment on number of luxury rooms available of the hotel
				h.availableLuxury[h.luxuryAvailable-1]=rmNum;//add back the room number (of the customer just checked-out) to the array containing all available luxury room numbers
			}
			if(strcmp(c[w].room.type,"Standard")==0){//if the room (of the customer just checked-out) is in standard type, execute this part
				h.standardAvailable+=1;//increment on number of standard rooms available of the hotel
				h.availableStandard[h.standardAvailable-1]=rmNum;//add back the room number (of the customer just checked-out) to the array containing all available standard room numbers
			}
			
			customerRemove(c[w].name,c[w].room.number);//call function to remove the customer record in the text file
			
			//remove the customer record in the program
			for(int r=w;r<custCount;r++){//loop r times, r refers to the difference of total number of customer currently in the hotel and the customer index of the customer checked-out
				if(strcmp(c[r+1].name,"")!=0){//while the customer checked-out is not the last customer in customer record, execute this part
					//move the record of the next customer to the current position (move the records forward)
					strcpy(c[r].name,c[r+1].name);
					strcpy(c[r].address,c[r+1].address);
					strcpy(c[r].date,c[r+1].date);
					strcpy(c[r].id,c[r+1].id);
					strcpy(c[r].room.bed,c[r+1].room.bed);
					strcpy(c[r].room.number,c[r+1].room.number);
					strcpy(c[r].room.type,c[r+1].room.type);
					c[r].room.charges=c[r+1].room.charges;
					c[r].room.fee=c[r+1].room.fee;				
				}
				else{//current position is the last record in the program (to be removed)
					//set all fields in this record to NULL or empty, indicating this record is empty
					strcpy(c[r].name,"");
					strcpy(c[r].address,"");
					strcpy(c[r].date,"");
					strcpy(c[r].id,"");
					strcpy(c[r].room.bed,"");
					strcpy(c[r].room.number,"");
					strcpy(c[r].room.type,"");
					c[r].room.charges=NULL;
					c[r].room.fee=NULL;	
				}
			}

			//after customer's record has successfully removed in both text file and the program
			customerWrite();//update the custoer record into the final destination on file location storing the customer text file
			printf("\n%s: You have successfully check-out. Hope to see you again!\n\n",na);//print result message with the customer name
			printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
			getch();
		}
		else if(input_Check(confirm)==0){//if user's input is "N", execute this part
			printf("You have cancelled to check-out.\n");//print message indicating that user had just cancelled to check-out
			printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
			getch();
		}
		else if(input_Check(confirm)==2){//if user's input is neither "Y" nor "N", execute this part
			printf("Wrong input, please try again.\n");//print error message showing that user's input is invalid
			check_out(cu,backupDate,w,custCount);//recall function
		}
	}
	else if(atoi(outday)<atoi(inday)){//if check-out date input is earlier than check-in date, execute this part
		printf("\nCheck-out date should not be earlier than check-in date.\n");//print error message showing that the check-out date input is invalid
		printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	else if(w==9999){//if customer does not has a record in hotel system, execute this part
		printf("Customer has not checked-in.\n");//print error message as customer had not checked-in
		printf("Press any key to get back to menu\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	else{//if input is invalid, execute this part
		printf("\nInvalid input. Heading back to menu\n");//print error message
	}
	return 0;
}

int customerAppend(char name[25], char adr[100], char id[50], char indate[10], char room[5]){//function to append a new row of customer record into the customer text file, with newly customer who had just checked-in
	FILE *fptr;//create file pointer
	fptr=fopen(currentPath,"a");//open the customer text file for appending a new record
    
	//append the record in the specified format, facilitates reading next time (the reason of this formatting will be explained in the report)
	fprintf(fptr,"%s#",name);
	fprintf(fptr,"%s#",adr);
    fprintf(fptr,"%s#",id);
	fprintf(fptr,"%s#",indate);
	fprintf(fptr,"%s#",room);
	fprintf(fptr,"0#0\n");

	fclose(fptr);//close file pointer
	customerCount++;//increment in customerCount, indicating the number of customer in the hotel now has just increased by 1
	int w=0;//local declaration, variable storing the customer index for returning
	w=findCustomer(name);//call function to find customer index and stored in variable w
	
	//copy the customer registered personal information in the program by the given customer index
	strcpy(c[w].address,adr);
	strcpy(c[w].date,indate);
	strcpy(c[w].id,id);
	strcpy(c[w].name,name);
	strcpy(c[w].room.number,room);
	c[w].room.charges=0;
	c[w].room.fee=0;

	return w;//return the customer index after the customer records are updated
}

int check_in(char cu[20],char date[10],int w){//called when user choose to check-in, customer name and check-in date should be given in the parameters when function is called
	//local declarations
	char type[10];//variable storing the room type chose by the customer
	char bed[20];//variable storing the bed type chose by the customer
	int twin=0;//variable used for checking the available number of rooms with a twin bed
	int single=0;//variable used for checking the available number of rooms with two single beds
	bool checkroom=false;//variable used for checking if rooms in required room type is available
	bool checkbed=false;//variable used for checking if rooms in required bed type is available
	char confirm[20];//variable storing input for confirmation

	//assumed that all even room numbers are with a Twin bed and those with odd room numbers are with two single beds
	//if customer (with a valid name(not empty or with spaces only)) has no record in hotel (has not checked-in) and there is/are room(s) available, execute this part
	if((h.roomAvailable>0)&&(w==9999)&&(strcmp(cu," ")!=0)){//if room(s) is/are available and customer is not occupying any rooms, do check-in
		printf("Room(s) available. Please choose room type (\"S\"- Standard/\"L\"- Luxury):\n");//ask for user's choice on room type
		gets(type);//get user's input (should be "L" or "S")
		if((strcmp(type,"L")==0)||(strcmp(type,"S")==0)){//if user's input is "L" or "S" (valid), execute this part
			if(strcmp(type,"S")==0){//if input is "S" (room type choice is Standard rooms)
				if(h.standardAvailable>0){//check if there is/are room(s) available in standard type, if room(s) available, execute this part
					strcpy(type,"Standard");//copy room type string to variable type
					
					//search and count for the number of rooms in twin bed and in two single beds separately
					for(int b=0;b<h.standardAvailable;b++){//loop b times, b refers to the number of rooms available in standard type (by looping over the array containing all standard available room numbers)
						if((h.availableStandard[b])%2==0){//if room number is even, execute this part
							twin+=1;//increment on variable twin, indicating one more room (with a Twin bed) is found to be available
						}
						else{//if room number is odd, execute this part
							single+=1;//increment on variable single, indicating one more room (with two single beds) is found to be available
						}
					}
					checkroom=true;//when checking process finish, set variable checkroom to true as an indication
				}
				else{//if number of standard rooms available is less than 1, execute this part
					printf("\nNo vacancy for standard rooms. Please try another room type.\n\n");//print message showing that there's no available room in standard type now (no vacancy)
					check_in(cu,date,w);//recall function
				}
			}
			else if(strcmp(type,"L")==0){//if input is "L" (room type choice is Luxury rooms)
				if(h.luxuryAvailable>0){//check if there is/are room(s) available in luxury type, if room(s) available, execute this part
					strcpy(type,"Luxury");//copy room type string to variable type
					for(int c=0;c<h.luxuryAvailable;c++){//loop c times, c refers to the number of rooms available in luxury type (by looping over the array containing all luxury available room numbers)
						if((h.availableLuxury[c])%2==0){//if room number is even, execute this part
							twin+=1;//increment on variable twin, indicating one more room (with a Twin bed) is found to be available
						}
						else{//if room number is odd, execute this part
							single+=1;//increment on variable single, indicating one more room (with two single beds) is found to be available
						}
					}
					checkroom=true;//when checking process finish, set variable checkroom to true as an indication
				}
				else{//if number of luxury rooms available is less than 1, execute this part
					printf("\nNo vacancy for luxury rooms. Please try another room type.\n\n");//print message showing that there's no available room in luxury type now (no vacancy)
					check_in(cu,date,w);//recall function
				}
			}

			//after checking on room type
			if(checkroom==true){//if checking on room types is done, execute this part
				printf("\n%s room available.\nPlease choose bed type (\"T\"- a Twin bed/\"S\"- two Single beds):\n",type);//ask for user's choice on bed type
				gets(bed);//get user's input (should be "T" or "S")
				if((strcmp(bed,"T")==0)||(strcmp(bed,"S")==0)){//if user's input is "T" or "S" (valid), execute this part
					if(strcmp(bed,"T")==0){//if input is "T" (bed type is a Twin bed)
						//use the above counting variable twin to determine if bed type chose is available
						if(twin>0){//if at least one room with a twin bed is available, execute this part
							strcpy(bed,"Twin");//copy the bed type string to the variable bed
							checkbed=true;//set variable checkbed to true if bed type is available
						}	
						else{//if number of rooms available (with a Twin bed) is less than 1, execute this part
							printf("\nNo vacancy in room with a twin bed. Please try another bed type.\n\n");//print message indicating that there's no rooms available with twin bed in the chosen room type
							check_in(cu,date,w);//recall function
						}
					}
					if(strcmp(bed,"S")==0){//if input is "S" (bed type is two single beds)
						//use the above counting variable single to determine if bed type chose is available
						if(single>0){//if at least one room with two single beds is available, execute this part
							strcpy(bed,"Two Single");//copy the bed type string to the variable bed
							checkbed=true;//set variable checkbed to true if bed type is available
						}
						else{//if number of rooms available (with two single beds) is less than 1, execute this part
							printf("\nNo vacancy in room with two single beds. Please try another bed type.\n\n");//print message indicating that there's no rooms available with twin bed in the chosen room type
							check_in(cu,date,w);//recall function
						}
					}

					//after checking on bed type
					if(checkbed==true){//if checking on bed types is done, execute this part
						printf("\nRoom type selected: %s\nBed type selected: %s\n\nConfirm? Y/N\n",type,bed);//ask for user confirmation on the chosen room type and bed type
						gets(confirm);//get user's input

						if(input_Check(confirm)==1){//if user's input is "Y", execute this part
							//user confirms to check-in, ask for more personal information for completing the registraton
							//another local declarations
							char adr[100];//variable storing customer address at registration
							char id[50];//variable storing customer id/passport at registration
							int room;//variable storing the room number generated
							char buffer[20];//variable used as a buffer in converting an integer to string format
							
							//generate new room number randomly with specified room type and bed type
							if(strcmp(type,"Luxury")==0){//if the room type chosen is luxury, execute this part
								room=rand_Room(h.availableLuxury,h.luxuryAvailable,bed);//call function to generate a room number randomly from the array containing all available room numbers in luxury type
							}
							if(strcmp(type,"Standard")==0){//if the room type chosen is standard, execute this part
								room=rand_Room(h.availableStandard,h.standardAvailable,bed);//call function to generate a room number randomly from the array containing all available room numbers in standard type
							}

							//continue registration on getting customer personal information
							bool inputCheck=false;//variable for checking if user's input on address and id/passport is valid
							while(inputCheck==false){//loop until inputs are valid
								printf("Customer address:\n");//ask for customer's address
								gets(adr);//get user's input
								printf("Customer ID/passport:\n");//ask for customer's id/passport
								gets(id);//get user's input
								if((strcmp(adr,"")==0)||(strcmp(adr," ")==0)){//user enter spaces or empty input
									printf("\nInvalid address.Please enter again\n\n");//print error message
								}
								else if((strcmp(id,"")==0)||(strcmp(id," ")==0)){//user enter spaces or empty input
									printf("\nInvalid ID/passport.Please enter again\n\n");//print error message
								}
								else{//if id or passport and address are both non-empty and non-spaces, execute this part
									inputCheck=true;//set variable inputCheck to true to end this while loop
								}
							}

							//if all inputs are valid, update the hotel and customer information
							w=customerAppend(cu,adr,id,date,itoa(room,buffer,10));//call function to append the new customer's record
							if(w!=9999){//w (customer index) successfully returned (if fail to do so, function returns 9999), execute this part
								h.occupied[(h.totalRoom-h.roomAvailable)]=room;//insert the room number of the customer just checked-in into the array containing all occupied room numbers
								removeRoomNum(h.availableRooms,h.roomAvailable,room);//call function to remove room number from array containing all available room numbers
								h.roomAvailable-=1;//decrement on hotel number of rooms available
								
								if(strcmp(type,"Luxury")==0){//if the room (of the customer just checked-in) is in luxury type, execute this part
									removeRoomNum(h.availableLuxury,h.luxuryAvailable,room);//call function to remove room number from array containing available luxury room numbers
									h.luxuryAvailable-=1;//decrement on number of luxury rooms available of the hotel
								}
								if(strcmp(type,"Standard")==0){//if the room (of the customer just checked-in) is in standard type, execute this part
									removeRoomNum(h.availableStandard,h.standardAvailable,room);//call function to remove room number from array containing available standard room numbers
									h.standardAvailable-=1;//decrement on number of standard rooms available of the hotel
								}
								
								strcpy(c[w].room.bed,bed);//copy customer's bed type to program's customer record
								strcpy(c[w].room.type,type);//copy customer's room type to program's customer record

								printf("\nSuccessfully checked-in, room number is %s",c[w].room.number);//print result message indicating customer has checked-in successfully
								printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
								getch();
							}
						}
						else if(input_Check(confirm)==0){//if user's input is "N", execute this part
							printf("Cancelled checked-in.");//print message indicating that user had just cancelled to check-in
							printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
							getch();
						}
						else if(input_Check(confirm)==2){//if user's input is neither "Y" nor "N", execute this part
							printf("Invalid input, please try again. (\"Y\" or \"N\")\n");//print error message showing that user's input is invalid
							check_in(cu,date,w);//recall function
						}
					}
				}
				else{//if user's input is neither "T" or "S", execute this part
					printf("Invalid input, please try again. (\"T\" or \"S\")\n\n");//print error message indicating that the input should only be either "S" or "T"
					check_in(cu,date,w);//recall function
				}
			}
		}
		else{//if user's input is neither "L" or "S", execute this part
			printf("Invalid input, please try again. (\"S\" or \"L\")\n\n");//print error message indicating that the input should only be either "S" or "L"
			check_in(cu,date,w);//recall function
		}
	}
	else if(h.roomAvailable==0){//if no rooms are available (no vacancy), execute this part
		printf("\nNo vacancy!\n");//print error message showing that no vacancy in hotel rooms
		printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	else if(strcmp(cu," ")==0){//if customer name is space(s), execute this part
		printf("\nInvalid name. (Please don't enter spaces!)\n");//print error message showing that user's name input is invalid
		printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	else if(w!=9999){//if customer index found (customer has a record in the hotel), execute this part
		printf("\nCustomer has already checked-in!\n");//print error message showing that customer has already checked-in
		printf("\nPress any key to get back to menu.\n\n");//works as a buffer to let user to see the result instead of heading back to menu immediately after printing
		getch();
	}
	else{//if input is invalid, execute this part
		printf("\nInvalid input. Heading back to menu\n");//print error message
	}
	return 0;
}

//main function
int main(){//simulating the front desk of a hotel, mainly used for displaying menu and provide service selection
	//load the current data (both hotel and customer information) into the program before menu display
	hotelInput();//load the hotel information, which is the updated room numbers and details of rooms saved in hotel text file
	customerCount=customerInput();//load the customers' information, which is the updated customers record saved in customer text file
						  		  //the returned value is the number of records read from the text file, which will be stored in variable customerCount (the global variable storing the current number of customers in the hotel)
	//printf("Version 1.0\n");
	//the following can be used at the first time of using this program (with no hotel text file)
	//the for loop is used to set the hotel rooms' statistics (room numbers, arrays, etc)
	//int p=0;
	//int q=0;
	//for(int x=0;x<h.totalRoom;x++){
	//	int rom=x+101;
	//	h.roomNumbers[x]=rom;//insert room numbers into an array contain all room numbers
	//	h.availableRooms[x]=rom;//insert rooms available numbers in array
	//	if(rom>=(101+(h.totalRoom-(h.totalRoom/2)))){
	//		h.availableLuxury[p]=rom;//insert luxury available
	//		h.luxuryRoomNumbers[p]=rom;
	//		p++;
	//	}
	//	else{
	//		h.availableStandard[q]=rom;//insert standard available
	//		h.standardRoomNumbers[q]=rom;
	//		q++;
	//	}
	//}
	//h.totalLuxury=p;
	//h.totalStandard=q;
	//h.luxuryAvailable=p;
	//h.standardAvailable=q;
	//h.roomAvailable=p+q;

	//the following can be used to remove the appropriate record(s) in the program given the customer text file (but without hotel text file)
	//remove occupied
	//for(int y=0;y<customerCount;y++){
	//	removeRoomNum(h.availableRooms,h.totalRoom,h.occupied[y]);
	//	if(h.occupied[y]>=(101+(h.totalRoom-(h.totalRoom/2)))){
	//		removeRoomNum(h.availableLuxury,(h.totalRoom/2),h.occupied[y]);//luxury
	//	}
	//	else{
	//		removeRoomNum(h.availableStandard,(h.totalRoom-(h.totalRoom/2)),h.occupied[y]);//standard
	//	}
	//}


	//print welcoming statement to introduce the system
	printf("***** Welcome to %s Management System *****\n",h.name);

	//repeat displaying menu with service selection until user decides to terminates the program
	//display menu and provide services available for selection
	//local declarations
	char role[20];//variable storing the user's name
	char cu[20];;//variable storing customer's name, used when service is selected
	bool check=false;//variable used to keep the while loop works
	//int n=0;
	char date[10];//variable storing date input
	int w=0;//variable storing returned customer index
	bool checkName=false;//variable used to keep while loop works until user input a valid name

	//keep asking for customer name or user name until the input is valid (not empty and not space(s) only)
	while(checkName==false){//while input is invalid
		printf("\nEnter customer name (Enter \"admin\" for information access): \n");//ask user to input his/her name
		gets(role);//get user's input
		if(strcmp(role,"")==0){//if input is empty, execute this part
			printf("Empty name is not allowed.\n");//print error message
		}
		else if(strcmp(role," ")==0){//if input is a space, execute this part
			printf("Only space(s) is not allowed.\n");//print error message
		}
		else{//if input is valid, execute this part
			checkName=true;//set variable checkName to true to quit while loop
		}
	}

	//loop the menu display providing services until user choose to quit the program
	while(quit==false){//while user do not choose to quit
		while(check==false){//while choice input correctly
			bool checkName1=false;//variable used to validates name input
			printf("\nYou are: %s\n",role);//print the customer name, like status
			printf("\nMenu: (Enter the number(or character) to select service)\n\n");//ask user to select the provided service
			
			if(strcmp(role,"admin")==0){//if the role is "admin", execute this part (admin is allowed to view hotel and customer information)
				w=9999;//set customer index to 9999, indicating current user is not a customer
				//provide services only available to admin, display only when current user is admin
				printf("100 - Hotel Information\n");//choose 100 to view hotel information
				printf("200 - Customers' information\n");//choose 200 to view customers' information
				printf("300 - Find room number\n");//choose 300 to find the room number by a customer name
				printf("400 - Find occupier\n");//choose 400 to find the occupier of a particular room
				printf("500 - Add a new room\n");//choose 500 to build a new room in the hotel due to expansion
				printf("600 - Remove a room\n");//choose 600 to remove an exist room in the hotel due to renovation
				printf("700 - Room Information\n\n");//choose 700 to view the information of a particular room (bed type, room type, status)
			}
			else{//if the role is not "admin", execute this part (a customer name)
				strcpy(cu,role);//copy the string to variable cu, stored as customer name
				w=findCustomer(cu);//call function to find if the customer has record in the hotel already, return 9999 if customer does not have a record
			}
			
			char ans[4];//variable storing user's choice on services
			int choice=0;//variable storing the choice entered by the user

			//display the services provided to the customer and admin
			printf("1 - Check-in\n");//choose 1 to check-in
			printf("2 - Check-out\n");//choose 2 to check-out
			printf("3 - Order meals (Dine)\n");//choose 3 to order set-meal(s)
			printf("4 - Make phone call\n");//choose 4 to make a phone call
			printf("5 - Buy a wine\n");//choose 5 to order wine
			printf("6 - Enter club\n");//choose 6 for club entrance
			printf("7 - Switch role\n");//choose 7 for role switching (should be chose when current user changes)
			printf("8 - Quit system\n\n");//choose 8 to quit system (hotel and customer text file will be updated here)
			
			gets(ans);//get user's input
			choice=atoi(ans);//convert user's input to integer format

			switch(choice){//use switch statement to determine which service is selected
			case 100://if user's input is 100
				//check=true;
				hotelInfo();//call function to print hotel information
				break;//break and back to menu

			case 200://if user's input is 200
				//check=true;
				customers(customerCount);//call function to print customers' information (sorted)
				break;//break and back to menu

			case 300://if user's input is 300
				//check=true;
				char ca[20];//variable storing customer name
				printf("Enter customer name to find his/her room number:\n");//ask user to input customer name for finding his/her room number
				gets(ca);//get user's input
				findRoom(ca);//call function to find room number
				break;//break and back to menu

			case 400://if user's input is 400
				//check=true;
				printf("Enter room number: \n");//ask user to input a room number to find the occupier
				char ga[5];//variable storing user's input
				gets(ga);//get user's input
				findOccupier(atoi(ga));//call function to find occupier
				break;//break and back to menu

			case 500://if user's input is 500
				//check=true;
				char type[10];//variable storing user's input
				printf("Enter room type to be build (\"Standard\" or \"Luxury\"):\n");//ask user to input the room type to be built
				gets(type);//get user's input
				buildNewRoom(type);//call function to build a new room
				break;//break and back to menu
			case 600://if user's input is 600
				printf("Enter room number to be removed: \n");//ask user to input a room number to be removed
				char gb[5];//variable storing user's input
				gets(gb);//get user's input
				removeRoom(atoi(gb));//call function to remove an exist room
				break;//break and back to menu

			case 700://if user's input is 700
				//check=true;
				printf("Enter room number for information access: \n");//ask user to input a room number for information access
				char gc[5];//variable storing user's input
				gets(gc);//get user's input
				roomInfo(atoi(gc));//call function to display room information
				break;//break and back to menu

			case 1://if user's input is 1
				//check=true;
				if(w==9999){//if customer does not have a record in the hotel, execute this part
					printf("Enter customer full name for check-in:\n");//ask user to input a customer name for check-in
					gets(cu);//get user's input
					if(strcmp(cu,"")==0){//if user's input is empty, execute this part
						printf("Invalid customer name.\nHeading back to menu.\n");//print error message
						break;//break and back to menu
					}
					w=findCustomer(cu);
				}
				printf("Check-in date (e.g. \"Day 3\"):\n");//ask user to input the check-in date
				gets(date);//get user's input
				int val;//variable used for checking if date input is valid
				val=checkDate(date);//call function to validates date input, returned value stored at variable val
				if(val==0){//if date input is valid (function returns 0), execute this part
					check_in(cu,date,w);//call function to let customer check-in
				}
				else{//if date input is invalid (function returns 1), execute this part
					printf("Invalid date input. Heading back to menu\n");//print error message
				}
				break;//break and back to menu

			case 2://if user's input is 2
				//check=true;
				if(w==9999){//if customer does not have a record in the hotel, execute this part
					printf("Enter customer full name for check-out:\n");//ask user to input a customer name for check-out
					gets(cu);//get user's input
					if(strcmp(cu,"")==0){//if user's input is empty, execute this part
						printf("Invalid customer name.\nHeading back to menu.\n");//print error message
						break;//break and back to menu
					}
					w=findCustomer(cu);//find customer index by the name input
					if(w==9999){//if customer cannot be found in the record, execute this part
						printf("Customer has not checked-in.\nHeading back to menu\n");//print error message showing that the customer has not checked-in
						break;//break and back to menu
					}
				}
				printf("Check-out date (e.g. \"Day 3\"):\n");//ask user to input check-out date for checking-out
				gets(date);//get user's input
				int valu;//variable used for validates date input
				valu=checkDate(date);//call function to validates date input, returned value stored at variable valu
				if(valu==0){//if date input is valid (function returns 0), execute this part
					check_out(cu,date,w,customerCount);//call function to let customer check-out
				}
				else{//if date input is invalid (function returns 1), execute this part
					printf("Invalid date input. Heading back to menu\n");//print error message
				}
				break;//break and back to menu

			case 3://if user's input is 3
				//check=true;
				if(w==9999){//if customer does not have a record in the hotel, execute this part
					printf("Enter customer full name ordering meals:\n");//ask user to input an exist customer name (valid name) for meal ordering
					gets(cu);//get user's input
					w=findCustomer(cu);//call function to find customer index
					if(w==9999){//if customer does not have a record in the hotel, execute this part
						printf("Invalid customer name.\nHeading back to menu\n");//print error message as customer record is not found
						break;//break and back to menu
					}
				}
				printf("Enter the number of set-meal(s) to be ordered:\n");//ask user to input the ordering amount of set-meal(s)
				char ge[2];//variable storing user's input
				gets(ge);//get user's input
				dine(cu,atoi(ge),w);//call function to order set-meals with the integer format of the ordering amount
				break;//break and back to menu

			case 4://if user's input is 4
				//check=true;
				if(w==9999){//if customer does not have a record in the hotel, execute this part
					printf("Enter customer full name for making phone-call:\n");//ask user to input an exist customer name (valid name) for dialing (making a phone call)
					gets(cu);//get user's input
					w=findCustomer(cu);//call function to find customer index
					if(w==9999){//if customer does not have a record in the hotel, execute this part
						printf("Invalid customer name.\nHeading back to menu\n");//print error message as customer record is not found
						break;//break and back to menu
					}
				}
				printf("Enter number of minutes for making a phone call:\n");//ask user to input the duration of the phone call
				char gd[3];//variable storing user's input
				gets(gd);//get user's input
				phone_call(cu,atoi(gd),w);//call function to make a phone call with the integer format of the duration
				break;//break and back to menu

			case 5://if user's input is 5
				//check=true;
				if(w==9999){//if customer does not have a record in the hotel, execute this part
					printf("Enter customer full name for wine ordering:\n");//ask user to input an exist customer name (valid name) for wine ordering
					gets(cu);//get user's input
					w=findCustomer(cu);//call function to find customer index
					if(w==9999){//if customer does not have a record in the hotel, execute this part
						printf("Invalid customer name.\nHeading back to menu\n");//print error message as customer record is not found
						break;//break and back to menu
					}
				}
				printf("Enter the number of wine to be ordered:\n");//ask user to input the amount of wine to bve ordered
				char gf[3];//variable storing user's input
				gets(gf);//get user's input
				drink(cu,atoi(gf),w);//call function for wine ordering with the integer format of the order amount
				break;//break and back to menu

			case 6://if user's input is 6
				//check=true;
				if(w==9999){//if customer does not have a record in the hotel, execute this part
					printf("Enter customer full name for club entrance:\n");//ask user to input an exist customer name (valid name) for club entrance
					gets(cu);//get user's input
					w=findCustomer(cu);//call function to find customer index
					if(w==9999){//if customer does not have a record in the hotel, execute this part
						printf("Invalid customer name.\nHeading back to menu\n");//print error message as customer record is not found
						break;//break and back to menu
					}
				}
				printf("Enter your membership number for entrance: (same as customer's room number)\n");//ask user to input his/her membership number for club entrance (assumed to be his/her room number)
				char gg[5];//variable storing user's input
				gets(gg);//get user's input
				enter_club(cu,atoi(gg),w);//call function for club entrance with the integer format of the membership number (room number)
				break;//break and back to menu

			case 7://if user's input is 7
				while(checkName1==false){//loop until the name input is valid (not empty and not space(s) only)
					printf("\nEnter your name (\"admin\" for admin):\n");//ask user to input the current user name
					gets(role);//get user's input
					if(strcmp(role,"")==0){//if user's input is empty, execute this part
						printf("Empty name is not allowed.\n");//print error message
					}
					else if(strcmp(role," ")==0){//if user's input a space, execute this part
						printf("Only space(s) is not allowed.\n");//print error message
					}
					else{//if user's input is valid, execute this part
						checkName1=true;//set variable checkName1 to true, indicating the input is valid to stop the while loop
					}
				}
				break;//break and back to menu

			case 8://if user's input is 8
				check=true;//set check to true to quit the while loop
				char confirm[20];//variable storing user's input
				printf("You are going to exit the system. Confirm? Y/N\n");//ask for user's confirmation to quit the system
				gets(confirm);//get user's input
				if(input_Check(confirm)==1){//if user input is "Y", execute this part
					customerWrite();//call function to write the updated customer records to the custoer text file
					hotelWrite();//call function to write the updated hotel information to the hotel text file
					printf("Thank you for using this system. See you again!\n");//print goodbye message
					quit=true;//set variable quit to true to end the while loop
					system("pause");
					exit(0);//quit the program
				}
				else if(input_Check(confirm)==0){//if user input on confirmation is "N", execute this part
					check=false;//set variable check to false as user choose not to quit the program
					printf("Heading back to menu\n");
					break;//break and back to menu
				}
				else if(input_Check(confirm)==2){//if user input on confirmation is neither "Y" nor "N", execute this part
					check=false;//set variable check to false as user entered an invalid input (not confirmed to quit yet)
					printf("Invalid input. Heading back to menu\n");
					break;//break and back to menu
				}
				break;//break and back to menu

			default://if user's input did not match any cases above, execute this part
				printf("Invalid request, please enter again.\n");//print error message, telling user that input is invalid
			}
		}
	}
	system("pause");
	return 0;
}

#include "parser.h"
#include <time.h>

extern "C" __declspec(dllexport) void ParseMessage(char* line, char* array[], char* output, BOOL debug) {
	char msg[32768];
	if (line == NULL || output == NULL) {
		exit;
	};
	char* token;
	char* new_token;
	//OutputDebugString(line);
	int index = 0;
	int points_index = 0;
	int string_index = 0;
	char tr[32768];
	char code;
	char privmsg_code[32768];
	char join_code[32768];
	char digit[32];
	token = strtok(line, " ");
	array[index] = token;
	//new_token = strtok(line, " ");										   
	while(token != NULL) {
		if(token[0] == ':' && index < 4) {
			array[index++] = token + 1;
		} else {
			array[index++] = token;
		};
		token = strtok(NULL, " ");
	};

	for (int i = 0; i < index; i++) {
		if(i < 2 && strlen(array[i]) < 4) {
			if(strcmp(array[1], "PRIVMSG") == 0) {
				sprintf(privmsg_code, "PRIVMSG"); 
			};
			if(strcmp(array[1], "JOIN") == 0) {
				sprintf(join_code, "Join"); 
			};
			if(isdigit(code) == 0) {
				sprintf(&code, "%s", array[i]);
			};
		};
		char current_time_ch[9];
		time_t current_time = time(NULL);
		strftime(current_time_ch, 9, "%H:%M:%S", localtime(&current_time));
		if (debug == TRUE) {
			OutputDebugString("WORD: [");
			OutputDebugString(array[i]);
			OutputDebugString("]\r\n");
			char index_str[40];
			char length_str[40];
			sprintf(index_str, "INDEX: %d\r\n", i); 
			sprintf(length_str, "LENGTH: %d\r\n", index); 
			OutputDebugString(index_str);
			OutputDebugString(length_str);
			OutputDebugString("\r\n");
		};
		if(i == 0 && strcmp(array[1], "PRIVMSG") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is PRIVMSG.");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			string_index += sprintf(output + string_index, "%s ", array[i]);
		} else if(i == 2 && strcmp(array[1], "PRIVMSG") == 0) {
			if(array[i][0] != '#') {
				string_index += sprintf(output + string_index, "(direct):");
			} else {
				string_index += sprintf(output + string_index, "(#%s):", array[i] + 1);
			};
		} else if(i == 0 && strcmp(array[1], "JOIN") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is JOIN.");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			string_index += sprintf(output + string_index, "%s joined the ", array[i]);
		} else if(i == 0 && strcmp(array[1], "QUIT") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is QUIT.");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			if(strlen(array[i]) < 3) {
				string_index += sprintf(output + string_index, "%s quited. (%s)\r\n", array[i], current_time_ch);
			} else {
				string_index += sprintf(output + string_index, "%s quited with reason: [", array[i]);
			};
		} else if(i >= 2 && strcmp(array[1], "QUIT") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is QUIT.");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			if(i > index - 2) {
				string_index += sprintf(output + string_index, "%s]. (%s)\r\n", array[i], current_time_ch);
			} else {
				string_index += sprintf(output + string_index, "%s ", array[i]);
			};
		} else if(i == 0 && strcmp(array[1], "PART") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is PART.");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			
			string_index += sprintf(output + string_index, "%s left the ", array[i]);
		} else if(i >= 3 && strcmp(array[1], "PART") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is PART.");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			if(i > index - 2) {
				string_index += sprintf(output + string_index, "%s]. (%s)\r\n", array[i], current_time_ch);
			} else {
				string_index += sprintf(output + string_index, "%s ", array[i]);
			};
		} else if(i == 0 && strcmp(array[1], "322") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is LIST (322).");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			
			string_index += sprintf(output + string_index, "%s\r\nTopic: ", array[3]);
		} else if(i >= 6 && strcmp(array[1], "322") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is LIST (322).");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			if(i > index - 2) {
				string_index += sprintf(output + string_index, "%s\r\nMembers: %d\r\n", array[i], atoi(array[4]));
				string_index += sprintf(output + string_index, "-----------------------------------\r\n", array[i]);
			} else {
				string_index += sprintf(output + string_index, "%s ", array[i]);
			};
		} else if(i == 2 && strcmp(array[1], "PART") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is PART.");
				OutputDebugString("\r\n");
			};
			if(index <= 4) {
				string_index += sprintf(output + string_index, "%s channel. (%s)\r\n", array[i], current_time_ch);
			} else {
				string_index += sprintf(output + string_index, "%s channel with reason: [", array[i]);
			};
		} else if(i == 2 && strcmp(array[1], "JOIN") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is JOIN.");
				OutputDebugString("\r\n");
			};
			string_index += sprintf(output + string_index, "%s channel. (%s)\r\n", array[i], current_time_ch);
		} else if(i == 0 && strcmp(array[1], "311") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is WHOIS (311).");
				OutputDebugString("\r\n");
			};
			string_index += sprintf(output + string_index, "%s (%s@%s)\r\nReal name: ", array[3], array[4], array[5]);
		} else if(i == 0 && strcmp(array[1], "317") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is WHOIS (317).");
				OutputDebugString("\r\n");
			};
			char login_timeft[48];
			char idle_timeft[12];
			time_t login_time = atoi(array[5]);
			time_t idle_time = atoi(array[4]);
			strftime(login_timeft, 48, "%Y-%m-%d %H:%M:%S", localtime(&login_time));
			strftime(idle_timeft, 12, "%H:%M:%S", localtime(&idle_time));
			string_index += sprintf(output + string_index, "%s idle, last logon time - %s\r\n", idle_timeft, login_timeft);
		} else if(i >= 7 && strcmp(array[1], "311") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is WHOIS (311).");
				OutputDebugString("\r\n");
			};
			if(i > index - 2) {
				string_index += sprintf(output + string_index, "%s\r\n", array[i]);
				string_index += sprintf(output + string_index, "-----------------------------------\r\n", array[i]);
			} else if(i == 7) {
				string_index += sprintf(output + string_index, "%s ", array[i] + 1);
			} else {
				string_index += sprintf(output + string_index, "%s ", array[i]);
			};
		} else if(i >= 4 && strcmp(array[1], "312") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is WHOIS (312).");
				OutputDebugString("\r\n");
			};
			
		} else if(i >= 3 && strcmp(array[1], "317") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is WHOIS (317).");
				OutputDebugString("\r\n");
			};
			
		} else if(i == 0 && strcmp(array[1], "353") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is TOPIC.");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			string_index += sprintf(output + string_index, "Members: ");
		} else if(i >= 5 && strcmp(array[1], "353") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is TOPIC.");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			if(i == 5 && i > index - 2) {
				string_index += sprintf(output + string_index, "%s\r\n", array[i] + 1);
			} else if(i > index - 2) {
				string_index += sprintf(output + string_index, "%s\r\n", array[i]);
			} else if(i == 5) {
				string_index += sprintf(output + string_index, "%s ", array[i] + 1);
			} else {
				string_index += sprintf(output + string_index, "%s ", array[i]);
			};
		} else if(i == 0 && strcmp(array[1], "TOPIC") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is TOPIC.");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			string_index += sprintf(output + string_index, "Topic: ");
		} else if(i >= 3 && strcmp(array[1], "TOPIC") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is TOPIC.");
				OutputDebugString("\r\n");
			};
			strtok(array[i], "!");
			if(i > index - 2) {
				string_index += sprintf(output + string_index, "%s\r\n", array[i]);
			} else {
				string_index += sprintf(output + string_index, "%s ", array[i]);
			};
		} else if(i < 3 && strlen(array[i]) < 4 && isdigit(code) != 0 && strcmp(array[1], "372") == 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is MOTD (372).");
				OutputDebugString("\r\n");
			};
			string_index += sprintf(output + string_index, "MOTD:");
		} else if(i < 3 && strlen(array[i]) < 4 && isdigit(code) != 0 && strcmp(array[1], "372") != 0 
			&& strcmp(array[1], "311") != 0 && strcmp(array[1], "312") != 0 && strcmp(array[1], "317") != 0 
			&& strcmp(array[1], "378") != 0 && strcmp(array[1], "318") != 0 && strcmp(array[1], "321") != 0 
			&& strcmp(array[1], "322") != 0 && strcmp(array[1], "323") != 0 && strcmp(array[1], "353") != 0
			&& strcmp(array[1], "366") != 0) {
			if (debug == TRUE) {
				OutputDebugString("Code is digit.");
				OutputDebugString("\r\n");
			};
			string_index += sprintf(output + string_index, "Code %s:", array[i]);
		} else if (i > index - 2 && strcmp(array[1], "JOIN") != 0 && strcmp(array[1], "QUIT") != 0 
			&& strcmp(array[1], "PART") != 0 && strcmp(array[1], "TOPIC") != 0 
			&& strcmp(array[1], "378") != 0 && strcmp(array[1], "312") != 0 
			&& strcmp(array[1], "317") != 0 && strcmp(array[1], "318") != 0 
			&& strcmp(array[1], "321") != 0 && strcmp(array[1], "322") != 0 
			&& strcmp(array[1], "323") != 0 && strcmp(array[1], "353") != 0 
			&& strcmp(array[1], "366") != 0) {
			if (debug == TRUE) {
				OutputDebugString("Code isn't digit. (i > index - 2)");
				OutputDebugString("\r\n");
			};
			if(strcmp(array[1], "PRIVMSG") == 0) {
				string_index += sprintf(output + string_index, " %s (%s)\r\n", array[i], current_time_ch);
			} else {
				string_index += sprintf(output + string_index, " %s\r\n", array[i]);
			};

		} else {
			if (i >= 1 && i <= index - 2) {
				if ((strcmp(array[1], "PRIVMSG") == 0) && i == 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages ((strcmp(array[1], \"PRIVMSG\") == 0) && i == 3)");
						OutputDebugString("\r\n");
					};
					string_index += sprintf(output + string_index, " %s", array[i]);
				} else if ((strcmp(array[1], "PRIVMSG") == 0) && i >= 4) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages ((strcmp(array[1], \"PRIVMSG\") == 0) && i >= 4)");
						OutputDebugString("\r\n");
					};
					string_index += sprintf(output + string_index, " %s", array[i]);
				} else if ((strcmp(array[1], "372") == 0) && i == 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages ((strcmp(array[1], \"372\") == 0) && i == 3)");
						OutputDebugString("\r\n");
					};
					string_index += sprintf(output + string_index, " %s", array[i]);
				} else if ((strcmp(array[1], "372") == 0) && i >= 4) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages ((strcmp(array[1], \"372\") == 0) && i >= 4)");
						OutputDebugString("\r\n");
					};
					string_index += sprintf(output + string_index, " %s", array[i]);
				} else if (strcmp(array[1], "JOIN") == 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"JOIN\") == 0 && i >= 3)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "QUIT") == 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"QUIT\") == 0 && i >= 3)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "PART") == 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"PART\") == 0 && i >= 3)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "TOPIC") == 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"TOPIC\") == 0 && i >= 3)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "311") == 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"311\") == 0 && i >= 3)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "312") == 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"312\") == 0 && i >= 3)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "318") == 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"318\") == 0 && i >= 3)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "378") == 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"378\") == 0 && i >= 3)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "321") == 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"321\") == 0 && i >= 3)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "322") == 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"322\") == 0 && i >= 3)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "323") == 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"323\") == 0 && i >= 3)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "TOPIC") == 0 && i == 2) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"TOPIC\") == 0 && i == 2)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "353") == 0 && i >= 2) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"TOPIC\") == 0 && i >= 2)");
						OutputDebugString("\r\n");
					};
				} else if (strcmp(array[1], "366") == 0 && i >= 2) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (strcmp(array[1], \"TOPIC\") == 0 && i >= 2)");
						OutputDebugString("\r\n");
					};
				} else if ((i == 1 && isdigit(code) == 0 && isdigit(code) == 0 && strcmp(array[1], "PRIVMSG") != 0 
					&& strcmp(array[1], "JOIN") != 0 && strcmp(array[1], "QUIT") != 0 && strcmp(array[1], "372") != 0 
					&& strcmp(array[1], "PART") != 0 && strcmp(array[1], "TOPIC") != 0 && strcmp(array[1], "321") != 0 
					&& strcmp(array[1], "322") != 0 && strcmp(array[1], "323") != 0 && strcmp(array[1], "353") != 0)
					&& strcmp(array[1], "366") != 0) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (i == 1 && isdigit(code) == 0 && strcmp(array[1], \"PRIVMSG\")\r\n"
						"!= 0 && strcmp(array[1], \"JOIN\") != 0 && strcmp(array[1], \"QUIT\") != 0 && strcmp(array[1], \"372\") != 0)");
						OutputDebugString("\r\n");
					};
					string_index += sprintf(output + string_index, "%s", array[i]);
				} else if ((i > 1 && isdigit(code) == 0)) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (i > 1 && isdigit(code) == 0 && strcmp(array[1], \"PRIVMSG\") != 0\r\n"
						"&& strcmp(array[1], \"JOIN\") != 0 && strcmp(array[1], \"QUIT\") != 0 && strcmp(array[1], \"372\") != 0)");
						OutputDebugString("\r\n");
					};
					string_index += sprintf(output + string_index, " %s", array[i]);
				} else if (isdigit(code) > 0 && i >= 3) {
					if (debug == TRUE) {
						OutputDebugString("Parsing messages (isdigit(code) > 0 && i >= 3 && strcmp(array[1], \"372\") != 0)");
						OutputDebugString("\r\n");
					};
					string_index += sprintf(output + string_index, " %s", array[i]);
				};		
			};
		};
	};


	//try {
		//output = tr;
	//} catch(...) {

	//};
	
};

extern "C" __declspec(dllexport) void ParseNamesMessage(char* line, char* array[], char* owners_array[], 
char* operators_array[], char* members_array[], int owners_count, int operators_count, int members_count, BOOL debug) {					  
	int owners_index = 0;
	int operators_index = 0;
	int members_index = 0;
	char debug_string[512];

	char* token;
	char* new_token;
	//OutputDebugString(line);
	int array_index = 0;
	int points_index = 0;
	int string_index = 0;
	char tr[32768];
	char code;
	char privmsg_code[32768];
	char join_code[32768];
	char digit[32];
	token = strtok(line, " ");
	array[array_index] = token;
	//new_token = strtok(line, " ");										   
	while(token != NULL) {
		if(token[0] == ':' && array_index < 6) {
			array[array_index++] = token + 1;
		} else {
			array[array_index++] = token;
		};
		token = strtok(NULL, " ");
	};

	for (int index = 5; index < array_index; index++) {
		if (debug == TRUE) {
			OutputDebugString("WORD: [");
			OutputDebugString(array[index]);
			OutputDebugString("]\r\n");
			char index_str[40];
			char length_str[40];
			sprintf(index_str, "INDEX: %d\r\n", index); 
			sprintf(length_str, "LENGTH: %d\r\n", array_index); 
			OutputDebugString(index_str);
			OutputDebugString(length_str);
			OutputDebugString("\r\n");
		};
		if(array[index] == "\0" || array[index] == NULL || strlen(array[index]) == 0) {
			
		} else if(array[index][0] == ':' && array[index][1] == '~') {
			if (debug == TRUE) {
				sprintf(debug_string, "Parsing NAMES messages (if owner): %s\r\n"
				"INDEX: %d\r\n", array[index], index);
				OutputDebugString(debug_string);
			};
			owners_array[owners_index++] = array[index] + 2;
		} else if(array[index][0] == ':' && array[index][1] == '@') {
			if (debug == TRUE) {
				sprintf(debug_string, "Parsing NAMES messages (if operator): %s\r\n"
				"INDEX: %d\r\n", array[index], index);
				OutputDebugString(debug_string);
			};
			operators_array[operators_index++] = array[index] + 2;
		} else if(array[index][0] == '~') {
			if (debug == TRUE) {
				sprintf(debug_string, "Parsing NAMES messages (if owner without \":\" mask): %s\r\n"
				"INDEX: %d\r\n", array[index], index);
				OutputDebugString(debug_string);
			};
			owners_array[owners_index++] = array[index] + 1;
		} else if(array[index][0] == '@') {
			if (debug == TRUE) {
				sprintf(debug_string, "Parsing NAMES messages (if operator without \":\" mask): %s\r\n"
				"INDEX: %d\r\n", array[index], index);
				OutputDebugString(debug_string);
			};
			operators_array[operators_index++] = array[index] + 1;
		} else if(array[index][0] == ':') {
			if (debug == TRUE) {
				sprintf(debug_string, "Parsing NAMES messages (if member): %s\r\n"
				"INDEX: %d\r\n", array[index], index);
				OutputDebugString(debug_string);
			};
			members_array[members_index++] = array[index] + 1;
		} else {
			if (debug == TRUE) {
				sprintf(debug_string, "Parsing NAMES messages (if member without \":\" mask): %s\r\n"
				"INDEX: %d\r\n", array[index], index);
				OutputDebugString(debug_string);
			};
			members_array[members_index++] = array[index];
		};
		owners_count = owners_index;
		operators_count = operators_index;
		members_count = members_index;
	};
	
	char parsing_result[32768];

	sprintf(parsing_result, "\r\nOWNERS: %d | OPERATORS: %d | MEMBERS: %d\r\n", owners_index, operators_index, members_index);
	OutputDebugString(parsing_result);

};

extern "C" __declspec(dllexport) void FreeArray() {

};

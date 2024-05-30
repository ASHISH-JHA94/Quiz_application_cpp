#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <chrono>


using namespace std;

// this is a string to lower case conversion for better comparison
string line_tolower(string line)
{
	int l=line.length();
	for (int i=0;i<l;i++)
	{
		line[i]=tolower(line[i]);
	}
	return line;
}

//timer function, chrono::steady_clock is a type indicating time
bool is_timeup(chrono::steady_clock::time_point start_time, int seconds) {
	//use the now() function to initialise the start time of the response
    auto current_time = chrono::steady_clock::now();
    //calculate the amount of time elapsed. duration_cast is used to store difference
    auto elapsed_seconds = chrono::duration_cast<chrono::seconds>(current_time - start_time).count();
    return (elapsed_seconds >= seconds);
}

//questions class
class questions {
	protected:
		//related questions data members
    	int question_id;
    	string question_text;
    	string correct_answer;
	public:
		//public data members
    	int marks;
    	int penalty;
    	
    	//questions constructor for making the object
    	questions(int question_id, string question_text, string correct_answer, int marks, int penalty)
        : question_id(question_id), question_text(question_text), correct_answer(correct_answer), marks(marks), penalty(penalty) {}

		//virtual function to display the question
    	virtual void display_question() {
        	cout << "Question " << question_id << ": " << question_text << endl;
    	}
		
		//virtual function to verify the answer
    	virtual int verify_answer(string user_answer, string player_name) {
    		user_answer=line_tolower(user_answer);
    	    if (user_answer == correct_answer) {
        	    cout << player_name << "'s answer for question " << question_id << " is correct. (+" << marks <<")"<<endl;
            	return marks;
        	} else {
            	cout << player_name << "'s answer for question " << question_id << " is wrong. (-" << penalty <<")"<<endl;
            	return -penalty;
        	}
    	}
};

class multiple_choice : public questions {
    //question choices data member
	vector<string> choices;

	public:
		//constructor passing the referenced members to the questions constructor
    	multiple_choice(int id, string text, string correct, int marks, int penalty, vector<string> &c)
        	: questions(id, text, correct, marks, penalty), choices(c) {}
        //override the display_question function for MCQ's
    	void display_question() {
        	questions::display_question();
        	for (size_t i = 0; i < choices.size(); ++i) {
            	cout << i + 1 << ". " << choices[i] << endl;
        	}
    	}
};

class true_false : public questions {
	//basic true false that directly inherits from the questions class
	public:
    	true_false(int id, string text, string correct, int m, int p)
        	: questions(id, text, correct, m, p) {
    	}
};

class fill_blank : public questions {
	//basic fill_blank class that inherits from the questions
	public:
    	fill_blank(int id, string text, string correct, int m, int p)
        	: questions(id, text, correct, m, p) {
    	}
};

class players {
	//players class
	public:
    	int player_id;
    	string player_name;
    	int score;
    	//players constructor
   	 	players(int id, string name)
        	: player_id(id), player_name(name), score(0) {
    	}
};

//leaderboard class to store and display the leaderboard
class leaderboard {
public:
    //data members
	vector<players> player_list;
    leaderboard(vector<players> player_vector):player_list(player_vector){}
    
    //print the leaderboard
    void build_leaderboard()
    {
    	cout<<"\n------Quiz Leaderboard------"<<endl;
    	int n=player_list.size();
    	//bubble sort in desc. order
    	for(int i=0;i<n;i++)
    	{
    		for(int j=0;j<n-i-1;j++)
    		{
    			if(player_list[j+1].score>player_list[j].score)
    			{
    				swap(player_list[j+1],player_list[j]);
				}
			}
		}
		//display the ranks
		for(int i=0;i<n;i++)
		{
			cout<<"Rank "<<i+1<<": "<<player_list[i].player_name<<" ---> "<<player_list[i].score<<endl;
		}
	}
};

void quiz() {
	//Quiz Questions------------------------------------------------------------------------------------
    vector<string> choices1;
    choices1.push_back("Kolkata");
    choices1.push_back("Mumbai");
    choices1.push_back("Chandigarh");
    choices1.push_back("Delhi");
    multiple_choice question1(1, "What is the financial capital of India?", "2", 4, 1, choices1);

    vector<string> choices2;
    choices2.push_back("Amazon");
    choices2.push_back("Mississippi");
    choices2.push_back("Nile");
    choices2.push_back("Yangzte");
    multiple_choice question2(2, "What is the longest river in the world?", "3", 4, 1, choices2);

    vector<string> choices3;
    choices3.push_back("Paris");
    choices3.push_back("London");
    choices3.push_back("Berlin");
    choices3.push_back("Rome");
    multiple_choice question3(3, "Which city is the capital of France?", "1", 4, 1, choices3);

    true_false question4(4, "The sun rises in the west.", "false", 2, 1);
    true_false question5(5, "Water boils at 100 degrees Celsius.", "true", 2, 1);
    true_false question6(6, "The moon is made of cheese.", "false", 2, 1);

    fill_blank question7(7, "The capital of Italy is ____.", "rome", 3, 1);
    fill_blank question8(8, "_______ is the largest planet in the solar system.", "jupiter", 3, 1);
    fill_blank question9(9, "____ is known as the Red Planet.", "mars", 3, 1);
    fill_blank question10(10,"The national space agency of India is ____","isro",3,1);
    
    vector<questions*> all_questions;
    all_questions.push_back(&question1);
    all_questions.push_back(&question2);
    all_questions.push_back(&question3);
    all_questions.push_back(&question4);
    all_questions.push_back(&question5);
    all_questions.push_back(&question6);
    all_questions.push_back(&question7);
    all_questions.push_back(&question8);
    all_questions.push_back(&question9);
    all_questions.push_back(&question10);
    //Quiz Questions-----------------------------------------------------------------------------------

	//welcome msg, and ask player count
	int player_count;
	cout<<"Welcome to the Quiz Game!\n"<<endl;
	cout<<"How many Players are playing? : ";cin>>player_count;
	cin.ignore();
	
	//make a player vector containing players
	vector<players> player_vector;
	string current_player_handle;
	for (int i=0;i<player_count;++i)
	{
		cout<<"Player "<<i+1<<": ";
		getline(cin, current_player_handle);
		players current_player(i,current_player_handle);
		player_vector.push_back(current_player);
	}
	
	//display what players are playing
	cout<<"\nPlayers Playing: "<<endl;
	for (int i=0;i<player_count;++i)
	{
		cout<<i+1<<": "<<player_vector[i].player_name<<endl;
	}
	
	//display quiz details
	cout<<"\n------------------------------------------------------------------"<<endl;
	cout<<"Quiz Details"<<endl;
	cout<<"There are 10 questions."<<endl;
	cout<<"3 Multiple Choice Questions"<<setw(10)<<"+4 -1"<<endl;
	cout<<"3 True and False Questions"<<setw(11)<<"+2 -1"<<endl;
	cout<<"4 Fill in the Blanks"<<setw(17)<<"+3 -1"<<endl;
	cout<<"--------------------------------------------------------------------"<<endl;
	cout<<"Only 5 seconds are provided to answer each question."<<endl;
	cout<<"If you take more than 5 seconds, your answer will not be counted."<<endl;
	cout<<"--------------------------------------------------------------------"<<endl;
	cout<<"Total Quiz Marks: 30"<<endl;
	cout<<"--------------------------------------------------------------------\n"<<endl;
	
	char begin;cout<<"Begin Quiz ?? Y/N: ";cin>>begin;
	int proceed=1;
	if(begin == 'Y'||begin == 'y'){
	//print the questions one by one
    for (int i = 0; i < all_questions.size(); ++i) { 
        //questions vector init and display
        if(proceed==1){
		questions* question = all_questions[i]; 
        question->display_question();
        
        //user answers vector creation
        vector<string> user_answers(player_count);
		cout<<endl;
        
        //ask answers to the questions
		for (int i = 0; i < player_count; ++i) {
            int user_choice;
            cout <<">> "<< player_vector[i].player_name << ", enter your choice or answer (within 5s): ";
            
            //start the timer
			auto start_time = chrono::steady_clock::now(); 
            
			cin >> user_answers[i];
			//check if the user had answered in time
      	  	if (is_timeup(start_time, 5)) {
        	    cout << "Time is up,"<<player_vector[i].player_name<<"! Your Answer will not be counted.( > 5s)" << endl;
        	    user_answers[i]="-1";
        	}
        }
		cout<<"\n-------------------------------------------------------"<<endl;
        for (int i = 0; i < player_count; ++i) {
            int user_score = question->verify_answer(user_answers[i], player_vector[i].player_name);
            player_vector[i].score += user_score;
        }
        cout<<"-------------------------------------------------------\n"<<endl;
        cout<<"Proceed? 0/1: ";cin>>proceed;
    	}else{
    		cout<<"Quiz has been stopped."<<endl;
    		break;
		}
    }
    cout<<endl;
	
	//display points scored
    cout << "Points scored by all participants are as follows: " << endl<<endl;
    for (int i = 0; i < player_count; ++i) {
        cout << player_vector[i].player_name << " has scored " << player_vector[i].score << "/30 points." << endl;
    }

	//display leaderboard for multiplayer
	if(player_count>1){
		leaderboard lb(player_vector);
		lb.build_leaderboard();
	}}
}

int main()
{
	quiz();
	return 0;
}

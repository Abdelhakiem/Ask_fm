#include <iostream>
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <deque>
#include <queue>
#include <stack>
#include<set>
#include<unordered_set>
#include<fstream>
#include<sstream>
#define users_path "files/users.txt"
#define quest_path "files/questions.txt"


using namespace std;
void init(){
    cin.tie(0);
    cin.sync_with_stdio(0);
    }

 //////////////////helper methods/////////////////////
 vector<string> readfilelines (string path)
 {
      vector<string> lines;
      string line;
      ifstream fin (path.c_str());
     if (fin.fail())
     {
         cout <<"error in downloading the file"<<endl;
     }
     while (getline(fin,line))
     {
         if (line.size()==0)continue;
         lines.push_back(line);

     }

     fin.close();
     return lines;

 }

 void writeLineFile (string path,vector<string> lines,bool append =true)
 {
     auto status = ios::in| ios::out|ios::app;
     if (!append)
     {
         status = ios::in| ios::out|ios::trunc;
     }
     ofstream fout (path.c_str(),status);
     if (fout.fail())
     {
         cout <<"error in downloading the file"<<endl;
     }
     for (auto i: lines)
     {
         fout<<i<<"\n";
     }
     fout.close();
 }
 vector <string> split_string(string s,string delimeter="," )
 {
     vector<string> str;
     string word;
     int pos=0;
     while((pos=(int)s.find(delimeter)) !=-1)
     {

         word=s.substr(0,pos);
         str.push_back(word);
         s.erase(0,s.find(delimeter)+delimeter.size());
     }str.push_back(s);
     return str;
 }

 int valid_choise(int start,int end_num)
 {
     int choise;
     cout <<"enter number in range("<<start<<" - "<<end_num<<")"<<endl;
        cin>>choise;
    if (start<=choise && choise<=end_num)
    {
        return choise;
    }
    else
    {
    cout <<"Invalid choise please try again"<<endl;
    return valid_choise(start,end_num);
    }
 }
 int display(vector <string> menu)
 {
     int x=0;
     for (auto i: menu)
     {
         cout <<++x<<") "<<i<<endl;
     }

     return valid_choise(1,menu.size());
 }
 //////////////////////////////////////////////////////////////////////////////////////////



 struct question
 {

     //               -1 if not                         1->anonymous
     int question_id,parent_id,to_user_id,from_user_id,is_anonymous;
     string question_text,answer_text;
     question()
     {
      question_id=parent_id=to_user_id=from_user_id=-1,is_anonymous=1;
     }
     question(string line)
     {
         vector <string> info=split_string(line);

      question_id=stoi(info[0]);
      parent_id=stoi(info[1]);
      from_user_id=stoi(info[2]);
      to_user_id=stoi(info[3]);
      is_anonymous=stoi(info[4]);
      question_text=info[5];
      answer_text=  info[6];
     }

     string Tostring()
     {
         ostringstream oss;
         oss<<question_id<<","<<parent_id
         <<","<<to_user_id<<","<< from_user_id
         <<","<<is_anonymous<<","<<question_text<<","<<answer_text;
         return oss.str();
     }

     void print_to_question()
     {
         if (parent_id!=-1) cout <<"   Thread: ";
         cout <<"Question Id("<<question_id<<")";
         if (is_anonymous!=1)cout <<" from user id ("<<from_user_id<<")";
         cout << "\n Question: "<<question_text<<endl<<"\n Answer: "<<answer_text<<endl;

     }
     void print_from_question()
     {
         cout <<"Question Id("<<question_id<<") ";
         if (is_anonymous==1)cout <<"!AQ";
         cout<<"to user id("<< to_user_id<<") "<<question_text;
         if (answer_text!= "")cout << "Answer: "<<answer_text<<endl;
         else cout << "Not answered yet"<<endl;

     }

       void print_feed_question()
     {
         if (parent_id!=-1) cout <<"   Thread Parent Question Id("<<parent_id<<")";
         cout <<"Question Id("<<question_id<<")";
         if (is_anonymous!=1)cout <<" from user id ("<<from_user_id<<")";
         cout <<" to user id ("<<to_user_id << ")\n Question: "<<question_text<<"\n Answer: "<<answer_text<<endl;

     }


 };


 struct user
 {
     int user_id,allow_anonymous_questions;
     string user_name,email,name,password;
     //questions from me
     vector <int>questions_id_from_me;
     //questions to me
     map<int,vector<int>> questions_threads;
     user()
     {
          user_id=-1,allow_anonymous_questions=1;
     }

     user(string line)
     {
         vector <string> str=split_string(line);
         user_id=stoi(str[0]);
        user_name=str[1];
         password=str[2];
          name=str[3];
         email=str[4];
         allow_anonymous_questions=stoi(str[5]);

     }
     string Tostring()
     {
        ostringstream oss;
         oss<<user_id<<","<<name
         <<","<<password<<","<< user_name
         <<","<<email<<","<<allow_anonymous_questions;
         return oss.str();
     }

   void Print() {
		cout << "User " << user_id << ", " << user_name << " " << password << ", " << name << ", " << email << endl;
	}


 };

  struct Question_manager
 {
     int last_id=0;
     map<int,question> question_id_object_map;
     map<int,vector<int>> question_threads_map;
Question_manager(){}
     void LoadDataBase()
     {
         question_id_object_map.clear();
    question_threads_map.clear();
         vector <string> lines=readfilelines(quest_path);
         for (auto &line: lines)
         {
             question ques(line);
             question_id_object_map[ques.question_id]=ques;
             last_id=max(last_id,ques.question_id);
             if (ques.parent_id==-1)
             {
                 question_threads_map[ques.question_id].push_back(ques.question_id);
             }
             else
            {
                question_threads_map[ques.parent_id].push_back(ques.question_id);
            }

         }
     }


     void filluserQuestions(user &user_1)
     {    //question_id_object_map, question_threads_map;
         //vetor : questions_id_from_me  map<int,vector<int>> questions_threads
         //  question_id,parent_id,to_user_id,from_user_id
         user_1.questions_threads.clear();
         user_1.questions_id_from_me.clear();
         for (auto &ids: question_threads_map)
         {
             for (auto &id: ids.second )
              {
                  question & Question=question_id_object_map[id];
                    //vetor : questions_id_from_me  map<int,vector<int>> questions_threads
                    if(user_1.user_id==Question.from_user_id)
                    {
                        user_1.questions_id_from_me.push_back(Question.question_id);
                    }
                    if(user_1.user_id==Question.to_user_id)
                    {
                        if(-1==Question.parent_id)
                        {
                            user_1.questions_threads[id].push_back(Question.question_id);
                        }
                        else
                            {
                               user_1.questions_threads[Question.parent_id].push_back(Question.question_id);
                            }
                    }

              }

         }


     }

     void PrintUserToQuestions(user &the_user)
     {cout<<endl;
        auto & threads=the_user.questions_threads;
        if (the_user.questions_threads.size()==0)
        {
            cout << "No Questions";
        }
        for (auto & pairs : threads)
           {
              for (auto & id : pairs.second)
                    {
                         question_id_object_map[id].print_to_question();

                    }
           }cout <<endl;
     }

     void PrintUserFromQuestions(user &the_user)
     {cout<<endl;
     if (the_user.questions_id_from_me.size()==0)
     {
         cout << "No questions";
     }
         auto & questions =the_user.questions_id_from_me;
         for (auto &id:questions)
         {
             question_id_object_map[id].print_from_question();
         }
         cout<<endl;
     }

     int EnterQuestionId(user & the_user)
     {
         cout << "Enter Question id or -1 to cancel: "<<endl;
         int id; cin>>id;
         if (id==-1)return -1;
         if (!question_id_object_map.count(id))
              {
                cout<<"Invalid id try again"<<endl;
                return EnterQuestionId(the_user);
              }
              question & Question=question_id_object_map[id];
        if(Question.to_user_id!=the_user.user_id)
             {
             cout <<"This Question Can't be accessed";
             return EnterQuestionId(the_user);
             }
             return id;
     }
     void AnswerQuestion(user & the_user)
     {
         int id =EnterQuestionId(the_user);
         question & Question= question_id_object_map[id];
         if (id==-1)return;

         Question.print_to_question();
         cout<<endl;
         if(Question.answer_text!="")
         {
             cout<<"Warning already answered Answer will be updated"<<endl;
         }
         cout<<"Enter answer: "<<endl;
         string text;
         cin.ignore();
        getline(cin, Question.answer_text);
     }


     void DeleteQuestion(user &the_user)
     {
        int id=EnterQuestionId(the_user);
        if (id==-1)return;
        vector <int>ids_to_remove;
        if(question_threads_map.count(id))
        {
            ids_to_remove=question_threads_map[id];
            question_threads_map.erase(id);
        }
        else
        {
            for (auto &pairs: question_threads_map)
            {
                vector<int> & vec=pairs.second;
                for(int i=0;i<(int)vec.size();i++)
                {
                   if(vec[i]==id)
                   {
                       ids_to_remove.push_back(id);
                       vec.erase(vec.begin()+i);
                       break;
                   }
                }
            }
        }

        for(auto & i: ids_to_remove)
        {
            question_id_object_map.erase(i);
        }
     }


     int Enter_thread()
     {
         int id;
         cout<<"For thread question: Enter Question id or -1 fro new question: "<<endl;
         cin>>id;
         if (id==-1)
         {
             return id;
         }

         if (!question_threads_map.count(id))
         {
             cout<<"Invalid parent question id ,please try again"<<endl;
             return Enter_thread();
         }
         return id;
     }


     void AskQuestion(user the_user, pair<int,int>to_user )
     {
         question Question;
        if (!to_user.second) {
			cout << "Note: Anonymous questions are not allowed for this user"<<endl;
			Question.is_anonymous = 0;
		}
		else
            {
                cout << "Is anonymous questions?: (0 or 1): "<<endl;
                  cin>>Question.is_anonymous;
            }

            Question.from_user_id =the_user.user_id;
            Question.to_user_id=to_user.first;
            Question.question_id=++last_id;
            Question.parent_id=Enter_thread();
            cin.ignore();
            getline(cin,Question.question_text);


            question_id_object_map[Question.question_id]=Question;
            if(Question.parent_id==-1)
            {
                question_threads_map[Question.question_id].push_back(Question.question_id);
            }
            else
            {
               question_threads_map[Question.parent_id].push_back(Question.question_id);
            }
     }

     void List_feed()
     {
         for (auto &feed: question_id_object_map)
         {
             if (feed.second.answer_text=="")continue;
             feed.second.print_feed_question();
         }
     }



     updateDataBase()
     {
         vector <string>lines;
         for (auto &pairs: question_id_object_map)
         {
             lines.push_back(pairs.second.Tostring());
         }
         writeLineFile(quest_path,lines,false);
     }
 };
  struct User_manager
 {
     int last_id;
     map <string,user> username_users_map;
     user active_user;
     User_manager()
     {
         last_id=0;
     }
     void LoadDataBase()
     {
         last_id=0;
         username_users_map.clear();

         vector <string> lines=readfilelines(users_path);
         for (auto &line: lines)
         {
             user the_user(line);
            username_users_map[the_user.user_name] =the_user;
             last_id=max(last_id,the_user.user_id);
         }
     }

     void systemAccess()
     {
         int choice =display({"Login", "Sign Up"});

         if (choice==1)
         {
          Login();
         }
         else
         {
           SignUp();
         }

     }

     void Login()
     {
         LoadDataBase();

         while (true)
         {
         cout<<endl<<"Enter user name & password: "<<endl;
         cin>>active_user.user_name>>active_user.password;
         if(!username_users_map.count(active_user.user_name))
         {
             cout<< "Invalid user name or Password "<<endl;
             continue;
         }
         else if(username_users_map[active_user.user_name].password!= active_user.password)
         {
             cout<< "Invalid user name or Password "<<endl;
             continue;
         }

                active_user=username_users_map[active_user.user_name];
                break;

         }

     }

     void SignUp()
     {
         user test;

         while (true)
         {
           cout<<"Enter User name. (No spaces): "<<endl;
           cin>>test.user_name;
           if(username_users_map.count(test.user_name))
           {
               cout <<"Already used. Try again"<<endl;
               continue;
           }
            break;
         }

         test.user_id=++last_id;
        cout << "Enter password: "<<endl;
        cin>>test.password;
         cout << "Enter name: "<<endl;
         cin>>test.name;
         cout << "Enter email: "<<endl;
		cin >> test.email;
         cout << "Allow anonymous questions? (0 or 1): "<<endl;
		cin >> test.allow_anonymous_questions;
         username_users_map[test.user_name]=test;
         active_user=test;
         updateDataBase(active_user);
     }



     pair <int,int> readUserId()
     {int id;
         cout<<"Enter User id or -1 to cancel: "<<endl;
         cin>>id;
         if(id==-1)
         {
             return make_pair(-1,-1);
         }
         for (auto &ids: username_users_map)
         {
             if(id== ids.second.user_id)
             {
                 return make_pair(id,ids.second.allow_anonymous_questions);
             }
         }
         cout<<"Invalid User Id.Try again"<<endl;
         return readUserId();
     }

     void list_system_users()
     {
         for (auto & user: username_users_map)
         {
             cout <<"ID: "<<user.second.user_id
             <<"         Name: "<<user.second.name<<endl;
         }
     }

     void updateDataBase(user the_user)
     {
         string line=the_user.Tostring();
         vector <string> lines(1,line);
         writeLineFile(users_path,lines);
     }

 };


 struct AskMe_sys
 {
     Question_manager question_manager;
     User_manager user_manager;

     void LoadDataBase(bool fill_questions=false)
     {
         question_manager.LoadDataBase();
         user_manager.LoadDataBase();
         if (fill_questions)
         {
             question_manager.filluserQuestions(user_manager.active_user);
         }
     }

     void run()
     {
        LoadDataBase();
        user_manager.systemAccess();
        question_manager.filluserQuestions(user_manager.active_user);

         vector<string>menu;
		menu.push_back("Print Questions To Me");
		menu.push_back("Print Questions From Me");
		menu.push_back("Answer Question");
		menu.push_back("Delete Question");
		menu.push_back("Ask Question");
		menu.push_back("List System Users");
		menu.push_back("Feed");
		menu.push_back("Logout");
		while(true)
        {
            LoadDataBase(true);
            user &current_user= user_manager.active_user;
          int choice =display (menu);

          if (choice == 1)
				{
				    question_manager.PrintUserToQuestions(current_user);
				}
			else if (choice == 2)
				{
				    question_manager.PrintUserFromQuestions(current_user);
				}
			else if (choice == 3)
                {
                    question_manager.AnswerQuestion(current_user);
                    question_manager.updateDataBase();
				}
				//do we have to fill user questions?
           else if (choice == 4)
                {
                    question_manager.DeleteQuestion(current_user);
                    question_manager.updateDataBase();
				}
           else if (choice == 5)
                {
                    auto user_pair= user_manager.readUserId();
                    if (user_pair.first!=-1)
                    {
                        question_manager.AskQuestion(current_user,user_pair);
                        question_manager.updateDataBase();
                    }
				}
			else if (choice == 6)
				{
				    user_manager.list_system_users();
				}
			else if (choice == 7)
				{
				    question_manager.List_feed();
				}
			else
				break;

        }run();
     }

 };



int main() {
    init();
    AskMe_sys my_sys;
    my_sys.run();

}
/*
for (int i=0;i<n; i++)
*/


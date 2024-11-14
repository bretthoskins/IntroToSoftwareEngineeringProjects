// CS216-001
// Author: Brett Hoskins
#include <iostream> 
#include <algorithm>
#include <limits>
#include <fstream> 
#include <string>
#include <cctype> 
#include <set>
#include <map>
#include <sstream>
#include "imdb.h"

using namespace std;

int main(int argc, char* argv[]){

	if (argc != 2) {
		cout << "Warning: need exactly one command line argument." << endl;
		cout << "Usage: " << argv[0] << " inputfile_name" << endl;
		return 1;
	}
	ifstream in_file;
	in_file.open(argv[1]);

	if (!in_file.good()) {
		cout << "Warning: cannot open file named " << argv[1] << "!" << endl;
		return 2;
	}

	IMDB cs216_imdb;

	while (!in_file.eof())
	{
		string line;
		getline(in_file, line);
		string actorName, movieTitle;
		istringstream iss(line);
		getline(iss, actorName, ';');

		iss>>ws;

		set<string> movies;

		while (getline(iss, movieTitle, ';')) {
			movies.insert(movieTitle);

			iss>>ws;
		}
		cs216_imdb.insert_an_actor(actorName, movies);
	}
	in_file.close();
// repeatedly ask the user for an input for actors or coactors, until they quit
	string option; 
	while (option != "Q" || option != "q") {
		cout << "This application stores information about Actors and their Movies, please choose an option (Enter Q or Q to quit):" << endl;
		cout << "1. Actors in Movies" << endl;
		cout << "2. Actors and Co-actors" << endl;
		cin >> option;
		
		if (option == "1") {
			// option 1, ask the user for the names of 2 movies, then repeatedly ask how they want the actors sorted 
			string movie1, movie2;
			cout << "Please input the first movie title: ";
			getline(cin,movie1);
			cout << "Please input the second movie title ";
			getline(cin, movie2);

			string matchedMovie1, matchedMovie2;
			matchedMovie1 = cs216_imdb.matchExistingMovie(movie1);
			matchedMovie2 = cs216_imdb.matchExistingMovie(movie2);
			if((matchedMovie1.length() > 0) && (matchedMovie2.length() > 0)) {
				cout << "Your input matches the following two movies: " << endl;
				cout << matchedMovie1 << endl;
				cout << matchedMovie2 << endl;
				cout << "Both movies are in the database, searching actors.." << endl;
			// repeats this menu until the user decides to quit 
				char usr_input;
				while (true) {
					cout << "Which set of actors would you like to see?" << endl;
					cout << "A (or a): Actors in either movie" << endl;
					cout << "C (or c): Actors in both movies" << endl;
					cout << "O (or o): Actors in one, but not both movies" << endl;
					
					cin >> usr_input;
					if(usr_input == 'Q' || usr_input == 'q'){
						break;
					} 		
			set<string> actors_in_movie1 = cs216_imdb.find_actors_in_a_movie(matchedMovie1);
			set<string> actors_in_movie2 = cs216_imdb.find_actors_in_a_movie(matchedMovie2); 
					// case for if the user wants to see the actors in either movie
					if (usr_input == 'A' || usr_input == 'a') {
						  set<string> all = actors_in_movie1;
		for (auto i = actors_in_movie2.begin(); i != actors_in_movie2.end(); i++) {
				all.insert(*i);	

							}
					cout << "ALl the actors in either movie:" << endl;
					for (auto it = all.begin(); it != all.end(); it++) {
						cout << *it << endl;
					}
					//case for if the user wants to see actors in both movies
					} else if (usr_input == 'C' || usr_input == 'c'){
						set<string> both;
						set_intersection(actors_in_movie1.begin(), actors_in_movie1.end(), actors_in_movie2.begin(), actors_in_movie2.end(), std::inserter(both, both.begin()));
						cout << "All the actors in both movies:" << endl;
						for (auto i = both.begin(); i != both.end(); i++){
							cout << *i << endl;
						}
					//case for if the user wants to see actors in one, not both movies
					} else if (usr_input == 'O' || usr_input == 'o'){
						set<string> difference;
						set_symmetric_difference(actors_in_movie1.begin(),actors_in_movie1.end(), actors_in_movie2.begin(), actors_in_movie2.end(), std::inserter(difference, difference.begin()));
						cout << "All the actors in one, not both movies: " << endl;
						for (auto i = difference.begin(); i != difference.end(); i++){
					cout << *i << endl;
						}
					} else {
						cout << "Invalid option!" << endl;						
					}
				}
				
			} else {
				cout << "Invalid movie title." << endl;
			}
		} else if (option == "2") {
			string actor_name;
			cout << "Finding the co-actors of the actor by typing his/her name: ";
			cin.ignore(256, '\n');
			getline(cin, actor_name);
			
			
			if(!cs216_imdb.isExistingActor(actor_name)) {
				cout << "The actor name you entered is not in the database." << endl;
				break;
			}

			set<string> movies_of_actor;
			movies_of_actor = cs216_imdb.find_movies_for_an_actor(actor_name);
			for(set<string>::iterator im = movies_of_actor.begin(); im != movies_of_actor.end(); im++) {
		cout << "The co-actors of " << actor_name << " in the movie \"" << *im << "\" are: " << endl;
		set<string> coactors = cs216_imdb.find_actors_in_a_movie(*im);
		for (set<string>::iterator ia = coactors.begin(); ia != coactors.end(); ia++) {
			if (*ia != actor_name) {
				cout << *ia << endl;
			}
		}
		cout << "*********************************************" << endl;
			}
		} else {
			cout << "Invalid option!" << endl;
			break;
		}
	}

cout << "Thank you for using my program, bye.." << endl;	
return 0;
}



#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include<unordered_map>
#include <queue>
#include <fstream>
#include <nlohmann/json.hpp>


class TimeSlot {
public:

	std::string day;
	std::string startTime;
	std::string endTime;


	TimeSlot() = default;

	TimeSlot(std::string _day, std::string _startTime, std::string _endTime) :
		day(_day), startTime(_startTime), endTime(_endTime) {}

	void displayInfo() const {

		std::cout << "Day: " << day << std::endl << "Start: " << startTime << std::endl << "End: " << endTime << std::endl;

	}


	nlohmann::json toJson() {

		return nlohmann::json{{"day", day}, { "startTime", startTime }, { "endTime", endTime }};

	}

	static TimeSlot fromJson(const nlohmann::json& j) {

		return TimeSlot(j["day"], j["startTime"], j["endTime"]);

	}


	bool operator==(const TimeSlot& newTileSlot) const {

		return day == newTileSlot.day && startTime == newTileSlot.startTime && endTime == newTileSlot.endTime;

	}
};


class Course {
public:

	std::string courseName;
	std::vector<TimeSlot> preferredTimeSlots;

	// Default constructor
	Course() = default;

	Course(std::string _courseName, const std::vector<TimeSlot>& preferredTimeSlots) :
		courseName(_courseName), preferredTimeSlots(preferredTimeSlots) {}


	void displayInfo() const{

		std::cout << "Course name: " << courseName << std::endl;

		if (!preferredTimeSlots.empty()) {

			std::cout << "Preferred Time Slots:" << std::endl;

			for (auto& time_slot : preferredTimeSlots) {

				time_slot.displayInfo();

			}
		}
		else {

			std::cout << "No preferred time slots." << std::endl;
		
		}

	}

	nlohmann::json toJson() const {

		nlohmann::json j;
		j["courseName"] = courseName;

		for (auto timeSlot : preferredTimeSlots) {

			j["preferredTimeSlots"].push_back(timeSlot.toJson());

		}
		return j;
	}

	static Course fromJson(const nlohmann::json& j) {

		std::vector<TimeSlot> preferredTimeSlots;
		
		for (const auto& item : j["preferredTimeSlots"]) {
		
			preferredTimeSlots.push_back(TimeSlot::fromJson(item));
		
		}
		return Course(j["courseName"], preferredTimeSlots);
	}

	bool operator==(const Course& newCourse) const {

		return  courseName == newCourse.courseName && preferredTimeSlots == newCourse.preferredTimeSlots;

	}

	bool operator== (const std::string& my_string) const{
		return my_string == courseName;
	}

};




class Instructor {
public:

	std::string name;
	std::vector<TimeSlot> availability;
	std::vector<Course> preferredCourses;


	// Default constructor
	Instructor() = default;

	Instructor(std::string _name, const std::vector<TimeSlot>& _availability, const std::vector<Course>& _preferredCourses) :
		name(_name), availability(_availability), preferredCourses(_preferredCourses) {}

	void displayInfo() const{

		std::cout << "Instructor's name: " << name << std::endl;

		if (!availability.empty()) {

			std::cout << "Availability:" << std::endl;

			for (auto& time_slot : availability) {
			
				time_slot.displayInfo();
			
			}
		}
		else {

			std::cout << "No availability information." << std::endl;
		
		}

		if (!preferredCourses.empty()) {
		
			std::cout << "Preferred Courses:" << std::endl;
			
			for (auto& course : preferredCourses) {
				
				course.displayInfo();

			}
		}
		else {

			std::cout << "No preferred courses." << std::endl;

		}
	}



	nlohmann::json toJson() const {

		nlohmann::json j;
		j["name"] = name;

		for (auto timeSlot : availability) {
		
			j["availability"].push_back(timeSlot.toJson());
		
		}

		for (auto course : preferredCourses) {
		
			j["preferredCourses"].push_back(course.toJson());
		
		}
		return j;
	}



	static Instructor fromJson(const nlohmann::json& j) {

		std::vector<TimeSlot> availability;

		for (const auto& item : j["availability"]) {
		
			availability.push_back(TimeSlot::fromJson(item));
		
		}

		std::vector<Course> preferredCourses;

		for (const auto& item : j["preferredCourses"]) {
		
			preferredCourses.push_back(Course::fromJson(item));
		
		}
		
		return Instructor(j["name"], availability, preferredCourses);
	}
};




class University {
public:

	std::vector<Course> courses;
	std::vector<Instructor> instructors;
	std::vector<TimeSlot> timeSlots;


	//default constructor
	University() = default;
	University(const std::vector<Course>& _courses, const std::vector<Instructor>& _instructors, const std::vector<TimeSlot>& _timeSlots) :
		courses(_courses), instructors(_instructors), timeSlots(_timeSlots) {}

	void addCourse(Course newCourse) {

		courses.push_back(newCourse);

	}

	void addInstructor(Instructor newInstructor) {

		instructors.push_back(newInstructor);

	}

	void addTimeSlot(TimeSlot newTimeSlot) {

		timeSlots.push_back(newTimeSlot);

	}


	void saveState(std::string filename) {

		nlohmann::json json_format_file;

		for (auto& course : courses) {

			json_format_file["courses"].push_back(course.toJson());
		
		}

		for (auto& instructor : instructors) {
		
			json_format_file["instructors"].push_back(instructor.toJson());
		
		}

		for (auto& timeSlot : timeSlots) {

			json_format_file["timeSlots"].push_back(timeSlot.toJson());
		
		}

		std::ofstream file;
		file.open(filename);
		if (file.is_open()) {

			file << json_format_file.dump();
			file.close();

		}
		else {

			std::cerr << "Unable to open file: " << filename << std::endl;
		
		}
	}

	void loadState(const std::string& filename) {

		nlohmann::json json_format_file;
		std::ifstream file;
		file.open(filename);
		if (file.is_open()) {

			file >> json_format_file;
			file.close();



			courses.clear();
			instructors.clear();
			timeSlots.clear();

			for (const auto& item : json_format_file["courses"]) {

				courses.push_back(Course::fromJson(item));
			
			}
			for (const auto& item : json_format_file["instructors"]) {
			
				instructors.push_back(Instructor::fromJson(item));
			
			}
			for (const auto& item : json_format_file["timeSlots"]) {
			
				timeSlots.push_back(TimeSlot::fromJson(item));
			
			}
		}
		else {
		
			std::cerr << "Unable to open file: " << filename << std::endl;
		
		}
	}




	void schedule() {

		std::map<std::string, std::vector<TimeSlot>> coursePreferredTimeMap;

		for (Course& course : courses) {

			coursePreferredTimeMap[course.courseName] = course.preferredTimeSlots;
		
		}

		std::map<std::string, std::vector<TimeSlot>> instructorAvailabilityMap;

		for (Instructor& instructor : instructors) {
		
			instructorAvailabilityMap[instructor.name] = instructor.availability;
		
		}

		std::map<std::string, std::vector<Course>> instructorPreferredCoursesMap;

		for (Instructor& instructor : instructors) {
		
			instructorPreferredCoursesMap[instructor.name] = instructor.preferredCourses;
		
		}

		


		//map for  final course schedule
		std::map<std::string, std::pair<std::string, TimeSlot>> finalSchedule;

		


		auto course = coursePreferredTimeMap.begin();
		while (course != coursePreferredTimeMap.end()) {

			bool courseScheduled = false;

			std::string my_course = course->first;
			
			for (auto& instructorCourses : instructorPreferredCoursesMap) {

				for (auto& preferredCourse : instructorCourses.second) {

					if (preferredCourse.courseName == my_course) {
					
						const std::string& instructor_name = instructorCourses.first;

						for (auto& preferredTime : coursePreferredTimeMap[my_course]) {

							for (auto& availabilityTime : instructorAvailabilityMap[instructor_name]) {
								if (preferredTime == availabilityTime) {

									finalSchedule[my_course] = { instructor_name, preferredTime };
									courseScheduled = true; 

									auto it = std::find(instructorAvailabilityMap[instructor_name].begin(), instructorAvailabilityMap[instructor_name].end(), preferredTime);

									if (it != instructorAvailabilityMap[instructor_name].end()) {

										instructorAvailabilityMap[instructor_name].erase(it);

									}

									course = coursePreferredTimeMap.erase(course);
									break;
								}
							}

							if (courseScheduled) {
								break;
							}


						}

						if (courseScheduled) {
							break;
						}

					}
				}

				if (courseScheduled) {
					break;
		        }

					
			}

			if (!courseScheduled) {
				++course;
			}
   
		}

			
		if(!coursePreferredTimeMap.empty()){

			auto course = coursePreferredTimeMap.begin();
			while (course != coursePreferredTimeMap.end()) {

				bool courseScheduled = false;

				for (auto& instructor : instructorAvailabilityMap) {

					for (auto& preferredTime : course->second) {


						if (std::find(instructor.second.begin(), instructor.second.end(),preferredTime) != instructor.second.end()) {

							auto it = std::find(instructor.second.begin(), instructor.second.end(), preferredTime);

							finalSchedule[course->first] = { instructor.first, preferredTime};
							courseScheduled = true;


							instructorAvailabilityMap[instructor.first].erase(it);
							course = coursePreferredTimeMap.erase(course);

							break;
						}
					}

					if (courseScheduled) {
						break;
					}

				}

				if (!courseScheduled) {
					++course;
				}
			}
			
		}


		if (!coursePreferredTimeMap.empty()) {

			auto course = coursePreferredTimeMap.begin();
			while (course != coursePreferredTimeMap.end()) {

				bool courseScheduled = false;

				for (auto& instructor : instructorPreferredCoursesMap) {

					if (std::find(instructor.second.begin(), instructor.second.end(), course->first) != instructor.second.end() && !instructorAvailabilityMap[instructor.first].empty()) {

						finalSchedule[course->first] = { instructor.first, instructorAvailabilityMap[instructor.first].front()};
						courseScheduled = true;

						instructorAvailabilityMap[instructor.first].erase(instructorAvailabilityMap[instructor.first].begin());
						course = coursePreferredTimeMap.erase(course);

						break;

					}

				}

				if (!courseScheduled) {

					++course;

				}

			}

	    }

		if (!coursePreferredTimeMap.empty()) {

			auto course = coursePreferredTimeMap.begin();
			while (course != coursePreferredTimeMap.end()) {

				bool courseScheduled = false;

				for (auto& instructor : instructorAvailabilityMap) {

					if (!instructor.second.empty()) {
					
						finalSchedule[course->first] = { instructor.first, instructor.second.front() };
						courseScheduled = true;

						instructor.second.erase(instructor.second.begin());
						course = coursePreferredTimeMap.erase(course);
						
						break;
					}

				}

			}

		}


		std::cout << "Final Schedule :" << std::endl << std::endl;
		for (auto& schedul : finalSchedule) {

			std::cout << "Course name : " << schedul.first << std::endl << "Instructor's name : " << schedul.second.first << std::endl << "Time Slot : ";
			schedul.second.second.displayInfo();
			std::cout << std::endl;

		}

		
	}



};


int main() {

	std::cout << "Test 1." << std::endl;


	TimeSlot ts1("Monday", "09:00", "10:00");
	TimeSlot ts2("Monday", "10:00", "11:00");
	TimeSlot ts3("Tuesday", "09:00", "10:00");
	TimeSlot ts4("Tuesday", "08:00", "09:00");
	TimeSlot ts5("Wednesday", "08:00", "09:00");
	TimeSlot ts6("Thursday", "13:00", "14:00");

	Course c1("Math", { ts1, ts2, ts5 });
	Course c2("Physics", { ts2, ts3 });
	Course c3("Chemistry", { ts1, ts3 });
	Course c4("English", { ts4, ts3 });
	Course c5("Biology", { ts6 });

	Instructor i1("A. Xachatryan", { ts1, ts3, ts6 }, { c1 });
	Instructor i2("E. Araqelyan", { ts2, ts4, ts5 }, { c2, c3 });
	Instructor i3("A. Aloyan", { ts1, ts5, ts6 }, { c3, c5 });
	Instructor i4("A. Sayamyan", { ts3, ts5, ts6 }, { c4 });


	University RAU;
	RAU.addCourse(c1);
	RAU.addCourse(c2);
	RAU.addCourse(c3);
	RAU.addCourse(c4);
	RAU.addCourse(c5);

	RAU.addInstructor(i1);
	RAU.addInstructor(i2);
	RAU.addInstructor(i3);
	RAU.addInstructor(i4);

	RAU.addTimeSlot(ts1);
	RAU.addTimeSlot(ts2);
	RAU.addTimeSlot(ts3);
	RAU.addTimeSlot(ts4);
	RAU.addTimeSlot(ts5);
	RAU.addTimeSlot(ts6);

	// Schedule courses
	RAU.schedule();

	RAU.saveState("timeslot.json");

	University newUni;
	newUni.loadState("timeslot.json");






	std::cout << "Test 2." << std::endl;

	TimeSlot ts_1("Monday", "09:00", "10:00");
	TimeSlot ts_2("Monday", "10:00", "11:00");
	TimeSlot ts_3("Tuesday", "09:00", "10:00");

	Course c_1("Geometry", { ts1, ts2 });
	Course c_2("Anatomy", { ts2, ts3 });
	Course c_3("Economics", { ts1, ts3 });


	Instructor i_1("Ani", { ts3 }, { c1 });
	Instructor i_2("Anahit", { ts1 }, { c2 });
	Instructor i_3("Armen", { ts2 }, { c3 });

	University EUA;
	EUA.addCourse(c_1);
	EUA.addCourse(c_2);
	EUA.addCourse(c_3);

	EUA.addInstructor(i_1);
	EUA.addInstructor(i_2);
	EUA.addInstructor(i_3);

	EUA.addTimeSlot(ts_1);
	EUA.addTimeSlot(ts_2);
	EUA.addTimeSlot(ts_3);

	//Schedule courses
	EUA.schedule();


	std::cout << "Test 3." << std::endl;


	TimeSlot t1("Monday", "09:00", "10:00");
	TimeSlot t2("Monday", "10:00", "11:00");
	TimeSlot t3("Tuesday", "09:00", "10:00");

	Course course1("Geometry", { t1, t2 });
	Course course2("Anatomy", { t2, t3 });
	Course course3("Economics", { t1, t3 });


	Instructor instructor1("Gayane", { t1, t3}, { course1 });
	Instructor instructor2("Davit", { t3 }, { course2 });
	Instructor instructor3("Aram", { t1, t2  }, { course3 });


	University YSU;
	YSU.addCourse(course1);
	YSU.addCourse(course2);
	YSU.addCourse(course3);

	YSU.addInstructor(instructor1);
	YSU.addInstructor(instructor2);
	EUA.addInstructor(instructor3);

	YSU.addTimeSlot(t1);
	YSU.addTimeSlot(t2);
	YSU.addTimeSlot(t3);

	YSU.schedule();
	return 0;
}
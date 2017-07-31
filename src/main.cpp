#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>
#include <cstdio>
#include <ctime>
#include <chrono>

// for convenience
using json = nlohmann::json;
using namespace std;
using namespace std::chrono;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid_steer;
  PID pid_speed;
  
  // time
  time_point<high_resolution_clock> now_time;
  
  /* PID Control for 'Speed Control' by Ziegler-Nichols' Ultimate Gain method */
  double reference_speed = 50.0; //
  double accelerate_time = 10.0; // 50[mph] <-> 10.0[sec], 60[mph] <-> 15.0[sec]
  double Ku_speed = 2.0; // P : 2.0
  double Tu_speed = 0.285; // 7 oscillations within 2.0 second

  // [P controller]
  // pid_speed.Init(0.50 * Ku_speed, 0.0, 0.0);
  // [PI Controller]
  pid_speed.Init(0.45*Ku_speed, 0.542*Ku_speed/Tu_speed, 0.0);
  // [PID Controller]
  // pid_speed.Init(0.60*Ku_speed, 1.2*Ku_speed/Tu_speed, 0.075*Ku_speed*Tu_speed);

  
  /* PID Control for 'Steering Control' by Ziegler-Nichols' Ultimate Gain method */
  double Ku_steer = 0.20; // // 20mph : 0.8 (narrowly okay), 40mph : 0.10
  double Tu_steer = 3.5; // 2 oscillations within 7.0 second

  // [P controller]
  // pid_steer.Init(0.50 * Ku_steer, 0.0, 0.0);
  // [PI Controller]
  // pid_steer.Init(0.45*Ku_steer, 0.542*Ku_steer/Tu_steer, 0.0);
  // [PID Controller]
  pid_steer.Init(0.60*Ku_steer, 1.2*Ku_steer/Tu_steer, 0.075*Ku_steer*Tu_steer); 

  
  h.onMessage([&pid_steer, &pid_speed, &now_time, &reference_speed , &accelerate_time ](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
      // "42" at the start of the message means there's a websocket message event.
      // The 4 signifies a websocket message
      // The 2 signifies a websocket event
            
      if (length && length > 2 && data[0] == '4' && data[1] == '2'){
	auto s = hasData(std::string(data).substr(0, length));
	if (s != "") {
	  auto j = json::parse(s);
	  std::string event = j[0].get<std::string>();
	  if (event == "telemetry") {

	    // j[1] is the data JSON object
	    double cte = std::stod(j[1]["cte"].get<std::string>());
	    double speed = std::stod(j[1]["speed"].get<std::string>());
	    // double angle = std::stod(j[1]["steering_angle"].get<std::string>());
	    
	    // prev_time as static
	    static time_point<high_resolution_clock> prev_time = high_resolution_clock::now();
	    static time_point<high_resolution_clock> start_time = high_resolution_clock::now();
	    
	    /* Update time*/
	    now_time = high_resolution_clock::now();
	    double total_time;
	    total_time = (double)duration_cast<milliseconds>(now_time - start_time).count() / (double) 1000;
	    double diff_time;
	    diff_time = (double)duration_cast<milliseconds>(now_time - prev_time).count() / (double) 1000;
	    if (diff_time < 0.001){
	      diff_time = 0.001;
	    }
	    prev_time = now_time;

	    /* Speed Control */
	    double throttle_value;	  
	    double e_speed;
	    double target_speed;

	    // the time variant reference speed (10 sec)
	    if (total_time < accelerate_time){
	      target_speed = reference_speed * total_time / accelerate_time;
	    }else{
	      target_speed = reference_speed;
	    }
	    e_speed = speed - target_speed;
	    pid_speed.UpdateError(e_speed, diff_time);
	    throttle_value = pid_speed.TotalError();

	    /* Steering Control */
	    double steer_value;
	    pid_steer.UpdateError(cte, diff_time);
	    steer_value = pid_steer.TotalError();
	    if(steer_value < -1.0){
	      steer_value = -1.0;
	    }else if(steer_value > 1.0){
	      steer_value = 1.0;
	    }

	    // web socket : send
	    json msgJson;
	    msgJson["steering_angle"] = steer_value;
	    msgJson["throttle"] = throttle_value;
	    auto msg = "42[\"steer\"," + msgJson.dump() + "]";
	    ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);

	    // for logging
	    std::cout << total_time << "," << speed << "," << steer_value << "," << diff_time << std::endl;

	  }
	} else {
	  // Manual driving
	  std::string msg = "42[\"manual\",{}]";
	  ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
	}
      }
    });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
      const std::string s = "<h1>Hello world!</h1>";
      if (req.getUrl().valueLength == 1)
	{
	  res->end(s.data(), s.length());
	}
      else
	{
	  // i guess this should be done more gracefully?
	  res->end(nullptr, 0);
	}
    });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
      std::cout << "Connected!!!" << std::endl;
    });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
      ws.close();
      std::cout << "Disconnected" << std::endl;
    });

  int port = 4567;
  if (h.listen(port))
    {
      std::cout << "Listening to port " << port << std::endl;
    }
  else
    {
      std::cerr << "Failed to listen to port" << std::endl;
      return -1;
    }
  h.run();
}

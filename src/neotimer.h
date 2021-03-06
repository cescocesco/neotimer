#ifndef NEOTIMER_H
#define NEOTIMER_H

#define NEOTIMER_INDEFINITE -1
#define NEOTIMER_UNLIMITED -1


#include <Arduino.h>

class Neotimer{
  public:
    //Methods
		Neotimer();
    Neotimer(long _t);      //Constructor
    ~Neotimer();            //Destructor
		
    void init();            //Initializations
    boolean done();         //Indicates time has elapsed
    boolean repeat(int times);
		boolean repeat(int times, long _t);
		boolean repeat();
		void repeatReset();
		boolean waiting();			// Indicates timer is started but not finished
    boolean started();			// Indicates timer has started
    void start();			//Starts a timer
    long stop();			//Stops a timer and returns elapsed time
    void restart();
    void reset();           //Resets timer to zero
    void set(long t);

  private:

    typedef struct myTimer{
      long time;
      long last;
      boolean done;
      boolean started;
    };

    struct myTimer _timer;
    boolean _waiting;
		int _times = NEOTIMER_UNLIMITED;
};

//Default constructor
Neotimer::Neotimer(){
	this->_timer.time = 1000; //Default 1 second interval if not specified
}

Neotimer::Neotimer(long _t){
  this->_timer.time = _t;
}

//Default destructor
Neotimer::~Neotimer(){
  
}

//Initializations
void Neotimer::init(){
  this->_waiting = false;
}

/*
 * Repeats a timer x times
 * Useful to execute a task periodically.
 * Usage: 
 * if(timer.repeat(10)){
 * 	  do something 10 times, every second (default)
 * }
 */
boolean Neotimer::repeat(int times){
	if(times != NEOTIMER_UNLIMITED){	
		// First repeat
		if(this->_times == NEOTIMER_UNLIMITED){
			this->_times = times;			
		}
		// Stop
		if(this->_times == 0){
			return false;
		}
		
		if(this->repeat()){
			this->_times--;
			return true;
		}
		return false;
	}
	return this->repeat();
}

/*
 * Repeats a timer x times with a defined period
 * Useful to execute a task periodically.
 * Usage: 
 * if(timer.repeat(10,5000)){
 * 	  do something 10 times, every 5 seconds
 * }
 */
boolean Neotimer::repeat(int times, long _t){
	this->_timer.time = _t;
	return this->repeat(times);
}

/*
 * Repeats a timer indefinetely
 * Useful to execute a task periodically.
 * Usage: 
 * if(timer.repeat()){
 * 	  do something indefinetely, every second (default)
 * }
 */
boolean Neotimer::repeat(){
  if(this->done()){
    this->reset();
    return true;
  }  
	if(!this->_timer.started){
		this->_timer.last = millis();
		this->_timer.started = true;
    this->_waiting = true;
  }	
  return false;	
}

void Neotimer::repeatReset(){
	this->_times = -1;
}

/*
 * Checks if timer has finished
 * Returns true if it finished
 */
boolean Neotimer::done(){
  if(!this->_timer.started) return false;
  if( (millis()-this->_timer.last) >= this->_timer.time){
    this->_timer.done = true;
    this->_waiting = false;
    return true;
  }
  return false;
}

/*
 * Sets a timer preset
 */
void Neotimer::set(long t){
  this->_timer.time = t;
}

/*
 * Resets a timer
 */
void Neotimer::reset(){
  this->stop();
  this->_timer.last = millis();
  this->_timer.done = false;
}

/*
 * Start a timer
 */
void Neotimer::start(){
	this->reset();
  this->_timer.started = true;
  this->_waiting = true;
}

/*
 * Stops a timer
 */
long Neotimer::stop(){
  this->_timer.started = false;
  this->_waiting = false;
  return millis()-this->_timer.last;
}

/*
 * Continues a stopped timer
 */
void Neotimer::restart(){
	if(!this->done()){
		this->_timer.started = true;
		this->_waiting = true;
	}
}

/*
 * Indicates if the timer is active
 * but has not yet finished.
 */
boolean Neotimer::waiting(){
  return (this->_timer.started && !this->done()) ? true : false;
}

boolean Neotimer::started(){
	return this->_timer.started;
}

#endif

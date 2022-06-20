#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
   long prev_idle_so_far = 0;
   long prev_active_so_far = 0;
};

#endif
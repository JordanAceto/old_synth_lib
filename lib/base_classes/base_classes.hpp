#ifndef BASE_CLASSES_H_INCLUDED
#define BASE_CLASSES_H_INCLUDED

class Is_Tickable
{
public:
  virtual void tick() = 0;
  virtual void setSampleRate(float sample_rate) { this->sample_rate = sample_rate; }

protected:
  float sample_rate = 0;
};

class Is_Processable
{
public:
  virtual void process() = 0;
};


#endif

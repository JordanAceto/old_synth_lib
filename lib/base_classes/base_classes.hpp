#ifndef BASE_CLASSES_H_INCLUDED
#define BASE_CLASSES_H_INCLUDED

class Is_Tickable
{
public:
  Is_Tickable() : sample_rate(0.0) {}
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

class Has_Frequency_Range
{
public:
  Has_Frequency_Range() : F1(1.0), F2(1.0) {}
  virtual void setFrequencyRange(float F1, float F2)
  {
    this->F1 = F1;
    this->F2 = F2;
  }

protected:
  float F1, F2;
};


#endif

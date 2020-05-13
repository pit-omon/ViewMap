#ifndef SCALER_H
#define SCALER_H
#include <QtGlobal>

class Scaler{
private:
    enum{MIN_LEVEL = 1};
    int m_minLevel;
    int m_maxLevel;
    int m_currentLevel;
    qreal m_scale;
public:
    Scaler(int levels, qreal step, int curr_level = MIN_LEVEL)
        :m_minLevel(MIN_LEVEL), m_maxLevel(levels), m_currentLevel(curr_level),
         m_scale(step){}
    bool levelUp();
    bool levelDown();
    bool isMaxlevel()const {return m_currentLevel == m_maxLevel;}
    qreal getScale()const {return m_scale;}
    virtual ~Scaler(){}
};

#endif // SCALER_H

#include "scaler.h"

bool Scaler::levelUp()
{
    if(m_currentLevel != m_maxLevel)
    {
        ++m_currentLevel;
        return true;
    }
    return false;
}
bool Scaler::levelDown()
{
    if(m_currentLevel != m_minLevel)
    {
        --m_currentLevel;
        return true;
    }
    return false;
}

#pragma once

#include "MCurveData.h"


class MCycleCurveData: public MCurveData
{
public:
    MCycleCurveData();
    void Reset() override;

    bool UpdateCurveData(qint64 elapsed);
    inline unsigned GetCurrentPoint(){ return m_uCurrentPoint; }

private:
    unsigned		 m_uCurrentPoint;
};

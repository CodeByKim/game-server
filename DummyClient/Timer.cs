using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class Timer
{
    private int mCurrentTick;
    private int mPrevTick;

    private int mDeltaTime;

    public Timer()
    {
        mCurrentTick = Environment.TickCount;
        mPrevTick = Environment.TickCount;
    }

    public void Update()
    {
        mCurrentTick = Environment.TickCount;
        mDeltaTime = mPrevTick - mCurrentTick;
        mPrevTick = mCurrentTick;
    }

    public int GetDeltaTime()
    {
        return mDeltaTime * -1;
    }
}

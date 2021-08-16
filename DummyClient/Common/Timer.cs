using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

class Timer
{
    [DllImport("winmm.dll", EntryPoint = "timeBeginPeriod")]
    public static extern uint timeBeginPeriod(uint uMilliseconds);

    [DllImport("winmm.dll", EntryPoint = "timeEndPeriod")]
    public static extern uint timeEndPeriod(uint uMilliseconds);

    //[DllImport("winmm.dll", EntryPoint = "timeGetTime")]
    //public static extern int timeGetTime();

    private long mCurrentTick;
    private long mPrevTick;

    private long mDeltaTime;

    public Timer()
    {
        timeBeginPeriod(1);

        //mCurrentTick = timeGetTime();
        //mPrevTick = timeGetTime();        
        mCurrentTick = DateTime.Now.Ticks;
        mPrevTick = DateTime.Now.Ticks;
    }

    ~Timer()
    {
        timeEndPeriod(1);
    }

    public void Update()
    {
        //mCurrentTick = timeGetTime();
        //mDeltaTime = mPrevTick - mCurrentTick;
        //mPrevTick = mCurrentTick;

        mCurrentTick = DateTime.Now.Ticks;
        mDeltaTime = mPrevTick - mCurrentTick;
        mPrevTick = mCurrentTick;
    }

    public float GetDeltaTime()
    {        
        return (float)((mDeltaTime * -1) / 10000000.0f);        
    }
}

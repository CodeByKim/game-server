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

    [DllImport("Kernel32.dll")]
    private static extern bool QueryPerformanceCounter(out long lpPerformanceCount);

    [DllImport("Kernel32.dll")]
    private static extern bool QueryPerformanceFrequency(out long lpFrequency);
    
    private long mDeltaTime;
    
    private long mFreq;
    private long mCurrentTime;
    private long mPrevTime;

    public Timer()
    {
        timeBeginPeriod(1);

        QueryPerformanceFrequency(out mFreq);

        QueryPerformanceCounter(out mCurrentTime);
        QueryPerformanceCounter(out mPrevTime);
    }

    ~Timer()
    {
        timeEndPeriod(1);
    }

    public void Update()
    {        
        QueryPerformanceCounter(out mCurrentTime);
        
        mDeltaTime = mPrevTime - mCurrentTime;
        mPrevTime = mCurrentTime;
    }

    public float GetDeltaTime()
    {        
        return (float)((mDeltaTime * -1) / 10000000.0f);        
    }
}

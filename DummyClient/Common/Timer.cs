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

    [DllImport("winmm.dll", EntryPoint = "timeGetTime")]
    public static extern uint timeGetTime();

    private uint mCurrentTick;
    private uint mPrevTick;

    private uint mDeltaTime;

    public Timer()
    {
        timeBeginPeriod(1);
        
        mCurrentTick = timeGetTime();
        mPrevTick = timeGetTime();

    }

    ~Timer()
    {
        timeEndPeriod(1);
    }

    public void Update()
    {        
        mCurrentTick = timeGetTime();
        mDeltaTime = mPrevTick - mCurrentTick;
        mPrevTick = mCurrentTick;
    }

    public int GetDeltaTime()
    {
        return (int)(mDeltaTime * -1);
    }
}

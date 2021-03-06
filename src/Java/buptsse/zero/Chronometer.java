package buptsse.zero;

import javax.swing.*;
import java.text.DecimalFormat;

public class Chronometer implements Runnable
{
    private JLabel TimeLabel;
    private volatile long ElapseTime = 0;
    private volatile boolean RunningFlag = false;
    private volatile boolean StopFlag = false;
    private Object DummyObject = new Object();
    private Thread ChronometerThread = null;

    public Chronometer(JLabel TimeLabel)
    {
        this.TimeLabel = TimeLabel;
    }

    public synchronized void start() {
        if(RunningFlag)
            return;
        ChronometerThread = new Thread(this);
        ChronometerThread.start();
    }

    @Override
    public void run() {
        long BaseTime = ElapseTime;
        long BeginTime = System.currentTimeMillis();
        StopFlag = false;
        RunningFlag = true;
        while(!StopFlag)
        {
            ElapseTime = System.currentTimeMillis() - BeginTime + BaseTime;
            updataLabel();
            try{
                ChronometerThread.sleep(3);
            }catch (Exception e){
                e.printStackTrace();
            }
        }
        StopFlag = false;
        RunningFlag = false;
        //System.out.println("Thread exit.");
    }

    public long getElapseTime()
    {
        synchronized (DummyObject) {
            return ElapseTime;
        }
    }

    public String getTimeString()
    {
        long second, minute, MilliSecond;
        synchronized (DummyObject) {
            return getTimeString(ElapseTime);
        }
    }

    public static String getTimeString(long time)
    {
        if(time < 0)
            return null;
        long second = 0, minute = 0, MilliSecond = 0;
        second = time / 1000;
        minute = second / 60;
        second %= 60;
        MilliSecond = (time % 1000) / 10;
        DecimalFormat formater = new DecimalFormat("00");
        return "" + formater.format(minute) + ':' + formater.format(second) + ":" +
                formater.format(MilliSecond);
    }

    private void updataLabel()
    {
        synchronized (TimeLabel)
        {
            TimeLabel.setText(getTimeString());
        }
    }

    public void pause()
    {
        if(RunningFlag)
        {
            StopFlag = true;
            try{
                ChronometerThread.join(100);
            }catch (Exception e){
                e.printStackTrace();
            }
        }
    }

    public void reset()
    {
        pause();
        ElapseTime = 0;
        updataLabel();
    }

}

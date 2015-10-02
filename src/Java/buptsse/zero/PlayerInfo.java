package buptsse.zero;

import java.util.Comparator;

public class PlayerInfo implements Comparable<PlayerInfo>
{
    public String PlayerName;
    public long TypingTime;
    public PlayerInfo(String PlayerName, long TypingTime)
    {
        this.PlayerName = PlayerName;
        this.TypingTime = TypingTime;
    }

    @Override
    public int compareTo(PlayerInfo player2) {
        if(this.TypingTime < player2.TypingTime)
            return -1;
        else if(this.TypingTime > player2.TypingTime)
            return 1;
        return 0;
    }
}

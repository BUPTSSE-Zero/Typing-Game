package buptsse.zero;

import javax.swing.*;
import javax.swing.border.BevelBorder;
import java.awt.*;

public class ScrollView extends JScrollPane
{
    private Component FillComponent = null;
    private int ContentHeight = -1;
    private Box ContentBox = null;
    private int CurrentHeight = 0;
    private static Color BorderColor = new Color(189, 189, 189);

    void setContentBox(Box box)
    {
        ContentBox = box;
        if(GlobalSettings.OSInfo == GlobalSettings.SystemPlatform.OS_WINDOWS)
        {
            box.setOpaque(true);
            box.setBackground(new Color(0xFAFAFA));
        }
        setBorder(BorderFactory.createLineBorder(BorderColor, 1));
    }

    void setContentHeight(int height)
    {
        ContentHeight = height;
    }


    @Override
    public void paint(Graphics g)
    {
        //System.out.println("Scroll View Repaint. Area Height=" + g.getClipBounds().height);
        if(ContentBox != null && g.getClipBounds().height != CurrentHeight)
        {
            CurrentHeight = g.getClipBounds().height;
            if(FillComponent != null)
            {
                ContentBox.remove(FillComponent);
                FillComponent = null;
            }
            if(g.getClipBounds().height > ContentHeight + 2)
            {
            	FillComponent = Box.createRigidArea(new Dimension(0, g.getClipBounds().height - ContentHeight - 2));
            	ContentBox.add(FillComponent);
            }
        }
        super.paint(g);
    }
}

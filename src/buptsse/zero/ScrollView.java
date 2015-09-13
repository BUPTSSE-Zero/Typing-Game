package buptsse.zero;

import javax.swing.*;
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
        g.setColor(BorderColor);
        g.drawLine(0, 0, g.getClipBounds().width - 1, 0);
        g.drawLine(0, g.getClipBounds().height - 1, g.getClipBounds().width, g.getClipBounds().height - 1);
    }

    @Override
    protected void paintBorder(Graphics g) {
    }
}

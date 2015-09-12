package buptsse.zero;

import jdk.internal.util.xml.impl.Input;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;

public class GameInterface
{
    private String PlayerName;
    private ArrayList<String> MultiRowText;
    private JFrame GameWindow = null;
    private Box MainBox = null;
    private int WindowTotalHeight = 0;

    private final int VERTICAL_MARGIN = 10;
    private final int HORIZONTAL_MARGIN = 10;
    private final int WINDOW_WIDTH = 1000;
    private static final float TITLE_FONT_SIZE = (float)30.0;

    private static ImageIcon IconOK = null;
    private static ImageIcon IconError = null;

    public static String LABEL_QUIT = "Quit";
    public static String LABEL_START = "Start";
    public static String LABEL_FINISH = "Finish";
    public static String LABEL_PAUSE = "Pause";
    public static String LABEL_REPLAY = "Replay";

    private void initWindow() {
        MainBox = Box.createVerticalBox();
        MainBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));

        //Title Area
        Box TitleBox = Box.createHorizontalBox();
        TitleBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
        TitleBox.setAlignmentY((float) 1.0);
        JLabel TitleLable = new JLabel(MainInterface.PRODUCT_NAME);
        TitleLable.setFont(MainInterface.GlobalFont.deriveFont(Font.BOLD, TITLE_FONT_SIZE));
        TitleBox.add(TitleLable);
        TitleBox.add(Box.createHorizontalGlue());
        JLabel PlayerNameLabel = new JLabel(PlayerName, new ImageIcon(GameInterface.class.getResource("res/icon/icon-person.png")), 0);
        PlayerNameLabel.setFont(MainInterface.GlobalFont);
        TitleBox.add(PlayerNameLabel);
        TitleBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
        JLabel TimeLabel = new JLabel("0:00", new ImageIcon(GameInterface.class.getResource("res/icon/icon-clock.png")), 0);
        TimeLabel.setFont(MainInterface.GlobalFont);
        TitleBox.add(TimeLabel);
        TitleBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
        MainBox.add(TitleBox);
        MainBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));
        GameWindow.add(MainBox, BorderLayout.NORTH);

        //Load Text
        int i;
        JLabel TextLabel[] = new JLabel[MultiRowText.size()];
        JTextField InputField[] = new JTextField[MultiRowText.size()];
        ScrollView ScrollTextView = new ScrollView();
        Box ScrollBox = Box.createVerticalBox();
        ScrollBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));
        int TotalHeight = 0;
        for(i = 0; i < MultiRowText.size(); i++)
        {
            Box TextLabelBox = Box.createHorizontalBox();
            TextLabelBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
            TextLabel[i] = new JLabel(MultiRowText.get(i));
            TextLabel[i].setFont(MainInterface.GlobalFont);
            TextLabelBox.add(TextLabel[i]);
            TextLabelBox.add(Box.createHorizontalGlue());

            Box InputFieldBox = Box.createHorizontalBox();
            InputFieldBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
            InputField[i] = new JTextField();
            InputField[i].setFont(MainInterface.GlobalFont);
            InputField[i].setMargin(new Insets(2, 4, 2, 4));
            InputFieldBox.add(InputField[i]);
            InputFieldBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
            JLabel IconIndicator = new JLabel(IconError);
            InputFieldBox.add(IconIndicator);
            InputFieldBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));

            ScrollBox.add(TextLabelBox);
            ScrollBox.add(Box.createVerticalStrut(VERTICAL_MARGIN / 2));
            ScrollBox.add(InputFieldBox);
            if(i < MultiRowText.size() - 1)
                ScrollBox.add(Box.createVerticalStrut(VERTICAL_MARGIN * 2));
        };
        ScrollBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));
        TotalHeight = ScrollBox.getMinimumSize().height;
        //System.out.println("Scroll Box Height=" + ScrollBox.getMinimumSize().height);
        ScrollTextView.setContentBox(ScrollBox);
        ScrollTextView.setContentHeight(TotalHeight);
        ScrollTextView.setViewportView(ScrollBox);
        GameWindow.add(ScrollTextView, BorderLayout.CENTER);

        //Button Area
        Box ButtonAreaBox = Box.createVerticalBox();
        ButtonAreaBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));
        Box ButtonBox = Box.createHorizontalBox();
        ButtonBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));

        JButton QuitButton = new JButton(LABEL_QUIT, new ImageIcon(GameInterface.class.getResource("res/icon/icon-quit.png")));
        QuitButton.setFont(MainInterface.GlobalFont);
        QuitButton.setMargin(new Insets(2, 10, 2, 10));
        ButtonBox.add(QuitButton);
        ButtonBox.add(Box.createHorizontalGlue());

        JButton ControlButton = new JButton(LABEL_START, new ImageIcon(GameInterface.class.getResource("res/icon/icon-start.png")));
        ControlButton.setFont(MainInterface.GlobalFont);
        ControlButton.setMargin(new Insets(2, 40, 2, 40));
        ButtonBox.add(ControlButton);
        ButtonBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));

        JButton FinishButton = new JButton(LABEL_FINISH, new ImageIcon(GameInterface.class.getResource("res/icon/icon-finish.png")));
        FinishButton.setFont(MainInterface.GlobalFont);
        FinishButton.setMargin(new Insets(2, 15, 2, 15));
        FinishButton.setEnabled(false);
        ButtonBox.add(FinishButton);
        ButtonBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));

        JButton ReplayButton = new JButton(LABEL_REPLAY, new ImageIcon(GameInterface.class.getResource("res/icon/icon-replay.png")));
        ReplayButton.setFont(MainInterface.GlobalFont);
        ReplayButton.setMargin(new Insets(2, 10, 2, 10));
        ButtonBox.add(ReplayButton);
        ButtonBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));

        ButtonAreaBox.add(ButtonBox);
        ButtonAreaBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));
        GameWindow.add(ButtonAreaBox, BorderLayout.SOUTH);

        WindowTotalHeight = MainBox.getMinimumSize().height + ScrollBox.getMinimumSize().height + ButtonAreaBox.getMinimumSize().height + 20;
    }


    GameInterface(String PlayerName, ArrayList<String> text)
    {
        this.PlayerName = PlayerName;
        this.MultiRowText = text;
        if(IconError == null)
            IconError = new ImageIcon(GameInterface.class.getResource("res/icon/icon-error.png"));
        if(IconOK == null)
            IconOK = new ImageIcon(GameInterface.class.getResource("res/icon/icon-ok.png"));
    }

    public void show()
    {
        GameWindow = new JFrame(MainInterface.PRODUCT_NAME);
        initWindow();
        GameWindow.setVisible(true);
        WindowTotalHeight += GameWindow.getInsets().top;
        if(WindowTotalHeight > MainInterface.ScreenSize.height - 50)
            WindowTotalHeight = MainInterface.ScreenSize.height - 50;
        GameWindow.setSize(WINDOW_WIDTH, WindowTotalHeight);
        GameWindow.setLocation((MainInterface.ScreenSize.width - GameWindow.getWidth()) / 2, (MainInterface.ScreenSize.height - GameWindow.getHeight()) / 2);
    }
}

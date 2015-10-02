package buptsse.zero;

import javax.swing.*;
import javax.swing.border.LineBorder;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

public class RankListInterface
{
    private ArrayList<PlayerInfo> PlayerList;
    private JFrame ParentWindow;
    private String PlayerName;
    private RankList Rank;

    private JFrame MainWindow;
    private Box TitleBox;
    private JTable RankTable;
    private Box ButtonAreaBox;
    private final int WINDOW_WIDTH = 800;
    private final int WINDOW_HEIGHT = 400;
    private final int VERTICAL_MARGIN = 10;
    private final int HORIZONTAL_MARGIN = 10;
    private final float TITLE_FONT_SIZE = 32;
    private final float TABLE_HEADER_FONT_SIZE = 20;
    private final int COLUMN_NUM = 3;
    public RankListInterface(ArrayList<PlayerInfo> PlayerList, RankList Rank, String PlayerName, JFrame ParentWindow)
    {
        this.PlayerList = PlayerList;
        this.ParentWindow = ParentWindow;
        this.PlayerName = PlayerName;
        this.Rank = Rank;
    }

    private void initWindow()
    {
        //Title
        TitleBox = Box.createVerticalBox();
        TitleBox.setAlignmentX(Box.CENTER_ALIGNMENT);
        TitleBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));
        Box TitleLabelBox = Box.createHorizontalBox();
        JLabel TitleLabel = new JLabel(GlobalSettings.WINDOW_TITLE + " - " + GlobalSettings.LABEL_RANK_LIST);
        TitleLabel.setFont(GlobalSettings.GlobalFont.deriveFont(Font.BOLD, TITLE_FONT_SIZE));
        TitleLabelBox.add(TitleLabel);
        TitleBox.add(TitleLabelBox);
        TitleBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));

        Box MD5LabelBox = Box.createHorizontalBox();
        MD5LabelBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
        MD5LabelBox.add(new JLabel("MD5:" + Rank.getTextMD5().toUpperCase()));
        MD5LabelBox.add(Box.createHorizontalGlue());
        MD5LabelBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
        TitleBox.add(MD5LabelBox);
        TitleBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));
        MainWindow.add(TitleBox, BorderLayout.NORTH);

        //Table
        JScrollPane ScrollTable = new JScrollPane();
        ScrollTable.setBorder(BorderFactory.createEmptyBorder());
        String RowData[][] = new String[PlayerList.size()][COLUMN_NUM];
        for(int i = 0; i < PlayerList.size(); i++)
        {
            RowData[i][0] = Integer.toString(i + 1);
            RowData[i][1] = new String(PlayerList.get(i).PlayerName);
            RowData[i][2] = Chronometer.getTimeString(PlayerList.get(i).TypingTime);
        }
        RankTable = new JTable(new MyTableModel(RowData, new String[] {GlobalSettings.LABEL_RANK, GlobalSettings.LABEL_PLAYER_NAME, GlobalSettings.LABEL_TYPING_TIME}));
        DefaultTableCellRenderer TableHeaderRenderer = (DefaultTableCellRenderer)RankTable.getTableHeader().getDefaultRenderer();
        TableHeaderRenderer.setHorizontalAlignment(SwingConstants.CENTER);
        TableHeaderRenderer.setOpaque(true);
        RankTable.getTableHeader().setFont(GlobalSettings.GlobalFont.deriveFont(Font.BOLD, TABLE_HEADER_FONT_SIZE));

        RankTable.setRowHeight(GlobalSettings.GlobalFont.getSize() + 5 * 2);
        DefaultTableCellRenderer TableRenderer = new DefaultTableCellRenderer(){
            @Override
            public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
                if (row % 2 == 0) {
                    setBackground(new Color(0xFAFAFA));
                }
                else if (row % 2 == 1) {
                    setBackground(new Color(0xF0F0F0));
                }
                if(PlayerList.get(row).PlayerName.equals(PlayerName))
                    setForeground(Color.RED);
                else
                    this.setForeground(Color.BLACK);
                return super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
            }
        };
        TableRenderer.setHorizontalAlignment(SwingConstants.CENTER);
        RankTable.setDefaultRenderer(Object.class, TableRenderer);
        RankTable.setFont(GlobalSettings.GlobalFont);
        RankTable.setOpaque(false);
        RankTable.setIntercellSpacing(new Dimension(0, 0));
        RankTable.setShowGrid(false);
        RankTable.setRowSelectionAllowed(false);

        ScrollTable.setBorder(new LineBorder(ScrollView.BorderColor));
        ScrollTable.setViewportView(RankTable);
        Box TableBox = Box.createHorizontalBox();
        TableBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
        TableBox.add(ScrollTable);
        TableBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
        MainWindow.add(TableBox, BorderLayout.CENTER);

        //Button
        ButtonAreaBox = Box.createVerticalBox();
        ButtonAreaBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));

        Box ButtonBox = Box.createHorizontalBox();
        ButtonBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
        JButton CleanButton = new JButton(GlobalSettings.LABEL_CLEAR, GlobalSettings.ICON_CLEAR);
        CleanButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                int ret = JOptionPane.showConfirmDialog(MainWindow, GlobalSettings.MESSAGE_QUERY_CLEAR_RANK_LIST, GlobalSettings.WINDOW_TITLE,
                        JOptionPane.OK_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE, GlobalSettings.ICON_DIALOG_QUESTION);
                if (ret == JOptionPane.OK_OPTION) {
                    PlayerList.clear();
                    Rank.updateRankList(PlayerList);
                    Rank.writeRankListFile();
                    MainWindow.dispose();
                }
            }
        });
        ButtonBox.add(CleanButton);

        ButtonBox.add(Box.createHorizontalGlue());
        JButton CloseButton = new JButton(GlobalSettings.LABEL_CLOSE, GlobalSettings.ICON_CLOSE);
        CloseButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                MainWindow.dispose();
            }
        });
        ButtonBox.add(CloseButton);
        ButtonBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
        ButtonAreaBox.add(ButtonBox);
        ButtonAreaBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));
        MainWindow.add(ButtonAreaBox, BorderLayout.SOUTH);
    }

    private class MyTableModel extends DefaultTableModel
    {
        MyTableModel(Object RowData[][], Object ColNames[])
        {
            super(RowData, ColNames);
        }

        @Override
        public boolean isCellEditable(int row, int column) {
            return false;
        }
    }


    public void show()
    {
        if(PlayerList == null || PlayerList.size() <= 0)
        {
            GlobalSettings.showMessageDialog(ParentWindow, GlobalSettings.MESSAGE_RANK_LIST_EMPTY, GlobalSettings.ICON_DIALOG_INFO);
            return;
        }
        MainWindow = new JFrame(GlobalSettings.WINDOW_TITLE);
        initWindow();
        MainWindow.setVisible(true);
        int TotalHeight = TitleBox.getMinimumSize().height + RankTable.getMinimumSize().height +
                          + RankTable.getTableHeader().getMinimumSize().height
                          + ButtonAreaBox.getMinimumSize().height  +MainWindow.getInsets().top + 10;
        if(TotalHeight > GlobalSettings.ScreenSize.height - 50)
            TotalHeight = GlobalSettings.ScreenSize.height - 50;
        MainWindow.setSize(WINDOW_WIDTH, TotalHeight);
        MainWindow.setLocation((GlobalSettings.ScreenSize.width - MainWindow.getWidth()) / 2, (GlobalSettings.ScreenSize.height - MainWindow.getHeight()) / 2);
    }
}

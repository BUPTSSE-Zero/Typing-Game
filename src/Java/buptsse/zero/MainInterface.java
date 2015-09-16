package buptsse.zero;

import java.awt.*;
import java.awt.event.*;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.ButtonGroup;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JRadioButton;
import javax.swing.JTextField;

public class MainInterface {
	private static JFrame MainWindow = null;
	private static Box MainBox = null;

	private static final int WINDOW_WIDTH = 700;
	private static final int VERTICAL_MARGIN = 10;
	private static final int HORIZONTAL_MARGIN = 10;
	private static final float TITLE_FONT_SIZE = (float)40.0;
	private static final int NUM_MAX_LENGTH = 8;

	
	private static void initWindow()
	{
		MainBox = Box.createVerticalBox();
		
		//Title
		Box TitleBox = Box.createVerticalBox();
		TitleBox.setAlignmentX((float)0.5);
		TitleBox.add(Box.createRigidArea(new Dimension(0, VERTICAL_MARGIN)));
		JLabel TitleLabel = new JLabel(GlobalSettings.WINDOW_TITLE);
		TitleLabel.setFont(GlobalSettings.GlobalFont.deriveFont(Font.BOLD, TITLE_FONT_SIZE));
		TitleBox.add(TitleLabel);
		TitleBox.add(Box.createRigidArea(new Dimension(0, VERTICAL_MARGIN)));
		MainBox.add(TitleBox);
		
		//Border
		Box DummyBox = Box.createHorizontalBox();
		DummyBox.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		Box TextFieldBox = Box.createVerticalBox();
		TextFieldBox.add(Box.createRigidArea(new Dimension(0, VERTICAL_MARGIN)));
		TextFieldBox.setBorder(BorderFactory.createLineBorder(new Color(189, 189, 189)));
		DummyBox.add(TextFieldBox);
		DummyBox.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		
		//Label Player Name and TextField
		Box PlayerNameInputBox = Box.createHorizontalBox();
		PlayerNameInputBox.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		JLabel LabelPlayerName = new JLabel(GlobalSettings.LABEL_PLAYER_NAME + ":");
		LabelPlayerName.setFont(GlobalSettings.GlobalFont);
		PlayerNameInputBox.add(LabelPlayerName);
		PlayerNameInputBox.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		final JTextField PlayerNameInput = new JTextField();
		PlayerNameInput.setMargin(new Insets(2, 4, 2, HORIZONTAL_MARGIN));
		PlayerNameInput.setFont(GlobalSettings.GlobalFont);
		PlayerNameInputBox.add(PlayerNameInput);
		PlayerNameInputBox.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		TextFieldBox.add(PlayerNameInputBox);
		TextFieldBox.add(Box.createRigidArea(new Dimension(0, VERTICAL_MARGIN)));
		
		
		//RadioButton1 and TextField
		Box Option1Box = Box.createHorizontalBox();
		Option1Box.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		final JRadioButton Option1Radio = new JRadioButton(GlobalSettings.LABEL_INTERNAL_TEXT + ":");
		Option1Radio.setFont(GlobalSettings.GlobalFont);
		Option1Radio.setSelected(true);
		Option1Box.add(Option1Radio);
		Option1Box.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		final JTextField Option1TextField = new JTextField();
		Option1TextField.setMargin(new Insets(2, 4, 2, 4));
		Option1TextField.setFont(GlobalSettings.GlobalFont);
		Option1Radio.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent e) {
				// TODO Auto-generated method stub
				if(e.getStateChange() == ItemEvent.DESELECTED)
					Option1TextField.setEnabled(false);
				else 
					Option1TextField.setEnabled(true);
			}
		});
		Option1Box.add(Option1TextField);
		Option1Box.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		TextFieldBox.add(Option1Box);
		TextFieldBox.add(Box.createRigidArea(new Dimension(0, VERTICAL_MARGIN)));
		
		//RadioButton2 and TextField
		Box Option2Box = Box.createHorizontalBox();
		Option2Box.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		final JRadioButton Option2Radio = new JRadioButton(GlobalSettings.LABEL_EXTERNAL_TEXT + "(.xml):");
		Option2Radio.setFont(GlobalSettings.GlobalFont);
		Option2Box.add(Option2Radio);
		Option2Box.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		final JTextField Option2TextField = new JTextField();
		Option2TextField.setMargin(new Insets(2, 4, 2, 4));
		Option2TextField.setFont(GlobalSettings.GlobalFont);
		Option2TextField.setEnabled(false);
		Option2Box.add(Option2TextField);
		Option2Box.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		final JButton BrowseButton = new JButton(GlobalSettings.LABEL_BROWSE + "...");
		BrowseButton.setIcon(GlobalSettings.ICON_OPEN);
		BrowseButton.setFont(GlobalSettings.GlobalFont);
		BrowseButton.setMargin(new Insets(2, 4, 2, 4));
		BrowseButton.setEnabled(false);
		Option2Radio.addItemListener(new ItemListener() {
			@Override
			public void itemStateChanged(ItemEvent e) {
				// TODO Auto-generated method stub
				if (e.getStateChange() == ItemEvent.DESELECTED) {
					Option2TextField.setEnabled(false);
					BrowseButton.setEnabled(false);
				} else {
					Option2TextField.setEnabled(true);
					BrowseButton.setEnabled(true);
				}
			}
		});
		final FileDialog FileBrowseDialog = new FileDialog(MainWindow);
		FileBrowseDialog.setMode(FileDialog.LOAD);
		FileBrowseDialog.setTitle(GlobalSettings.LABEL_BROWSE);
		FileBrowseDialog.setFilenameFilter(new FilenameFilter() {
			@Override
			public boolean accept(File dir, String name) {
				// TODO Auto-generated method stub
				if (name.toLowerCase().endsWith(".xml"))
					return true;
				return false;
			}
		});
		BrowseButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				FileBrowseDialog.setVisible(true);
				if(FileBrowseDialog.getFile() != null)
				{
					if(FileBrowseDialog.getFile().contains(File.pathSeparator))
						Option2TextField.setText(FileBrowseDialog.getFile());
					else
						Option2TextField.setText(FileBrowseDialog.getDirectory() + FileBrowseDialog.getFile());
				}
			}
		});
		Option2Box.add(BrowseButton);
		Option2Box.add(Box.createRigidArea(new Dimension(HORIZONTAL_MARGIN, 0)));
		TextFieldBox.add(Option2Box);
		TextFieldBox.add(Box.createRigidArea(new Dimension(0, VERTICAL_MARGIN)));
		
		ButtonGroup OptionRadioGroup = new ButtonGroup();
		OptionRadioGroup.add(Option1Radio);
		OptionRadioGroup.add(Option2Radio);
		MainBox.add(DummyBox);
		MainBox.add(Box.createVerticalStrut(VERTICAL_MARGIN));
		
		//Button Area
		Box ButtonBox = Box.createHorizontalBox();
		ButtonBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
		JButton AboutButton = new JButton(GlobalSettings.LABEL_ABOUT, GlobalSettings.ICON_ABOUT);
		AboutButton.setFont(GlobalSettings.GlobalFont);
		AboutButton.setMargin(new Insets(2, 10, 2, 10));
		AboutButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				JOptionPane.showMessageDialog(MainWindow, GlobalSettings.PRODUCT_NAME + "\n" + "Powered by Java Swing\nCopyright© 2015 BUPTSSE-Zero", GlobalSettings.LABEL_ABOUT,
						JOptionPane.INFORMATION_MESSAGE, GlobalSettings.ICON_DIALOG_INFO);
			}
		});
		ButtonBox.add(AboutButton);
		final JButton EnterButton = new JButton(GlobalSettings.LABEL_ENTER, GlobalSettings.ICON_ENTER);
		EnterButton.setFont(GlobalSettings.GlobalFont);
		EnterButton.setMargin(new Insets(2, 30, 2, 30));
		ButtonBox.add(Box.createHorizontalGlue());
		ButtonBox.add(EnterButton);
		ButtonBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
		JButton ExitButton = new JButton(GlobalSettings.LABEL_EXIT, GlobalSettings.ICON_EXIT);
		ExitButton.setFont(GlobalSettings.GlobalFont);
		ExitButton.setMargin(new Insets(2, 10, 2, 10));
		ExitButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				int ret = JOptionPane.showConfirmDialog(MainWindow, GlobalSettings.MESSAGE_QUERY_EXIT, GlobalSettings.WINDOW_TITLE,
						JOptionPane.OK_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE, GlobalSettings.ICON_DIALOG_QUESTION);
				if (ret == JOptionPane.OK_OPTION) {
					MainWindow.dispose();
					System.exit(0);
				}
			}
		});
		ButtonBox.add(ExitButton);
		ButtonBox.add(Box.createHorizontalStrut(HORIZONTAL_MARGIN));
		MainBox.add(ButtonBox);
		MainBox.add(Box.createVerticalStrut(VERTICAL_MARGIN + 5));
		MainWindow.add(MainBox, BorderLayout.NORTH);

		Option1TextField.addKeyListener(new KeyListener() {
			@Override
			public void keyTyped(KeyEvent e) {
				if (e.getKeyChar() == KeyEvent.VK_ENTER)
					EnterButton.doClick();
				if ((e.getKeyChar() < KeyEvent.VK_0 || e.getKeyChar() > KeyEvent.VK_9) && e.getKeyChar() != KeyEvent.VK_BACK_SPACE)
					e.consume();                        //exclude the non-number input
				if (Option1TextField.getText().length() >= NUM_MAX_LENGTH && e.getKeyChar() != KeyEvent.VK_BACK_SPACE)
					e.consume();
			}

			@Override
			public void keyPressed(KeyEvent e) {}

			@Override
			public void keyReleased(KeyEvent e) {}

		});

        PlayerNameInput.addKeyListener(new KeyListener() {
            @Override
            public void keyTyped(KeyEvent e) {
                if(e.getKeyChar() == KeyEvent.VK_ENTER)
                    EnterButton.doClick();
            }

            @Override
            public void keyPressed(KeyEvent e) {}

            @Override
            public void keyReleased(KeyEvent e) {}
        });

		EnterButton.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				String TempStr = PlayerNameInput.getText();
				int SpaceCount = 0;
				for(int i = 0; i < TempStr.length(); i++)
				{
					if(TempStr.charAt(i) == ' ')
						SpaceCount++;
				}
				if(SpaceCount == TempStr.length())
				{
					showErrorDialog(GlobalSettings.MESSAGE_PLAYER_NAME_BLANK);
					PlayerNameInput.requestFocus();
					PlayerNameInput.selectAll();
					return;
				}
				TextFileParser parser = new TextFileParser();
				String FilePath = null;
				if(Option1Radio.isSelected() && Option1TextField.getText().length() > 0)
				{
					int IDNum = 0;
					try{
						IDNum = Integer.parseInt(Option1TextField.getText());
					}catch (Exception exception){
						showErrorDialog(GlobalSettings.MESSAGE_ID_NUMBER_INVALID);
						return;
					}
					FilePath = "res/text/text" + IDNum + ".xml";
					if(parser.parseFile(MainInterface.class.getResourceAsStream(FilePath)) == false)
					{
						showErrorDialog(GlobalSettings.MESSAGE_FILE_OPEN_FAILD + "\n" + FilePath);
						return;
					}
				}
				else if(Option2Radio.isSelected() && Option2TextField.getText().length() > 0)
				{
					boolean ParseResult;
					FilePath = Option2TextField.getText();
					try{
						ParseResult = parser.parseFile(new FileInputStream(FilePath));
					}catch (Exception exception) {
						exception.printStackTrace();
						ParseResult = false;
					}
					if(!ParseResult){
						showErrorDialog(GlobalSettings.MESSAGE_FILE_OPEN_FAILD + "\n" + FilePath);
						return;
					}
				}
				else
					return;
				ArrayList<String> TextList = parser.getMultiRowText();
				if(TextList.isEmpty())
				{
					showErrorDialog(GlobalSettings.MESSAGE_TEXT_EMPTY + "\n" + FilePath);
					return;
				}
				new GameInterface(TempStr, TextList).show();
			}
		});
	}

	private static void showErrorDialog(String message)
	{
		JOptionPane.showMessageDialog(MainWindow, message, GlobalSettings.WINDOW_TITLE, JOptionPane.CLOSED_OPTION,
										GlobalSettings.ICON_DIALOG_ERROR);
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		GlobalSettings.loadIcon();
		GlobalSettings.setUI();
		MainWindow = new JFrame();
		MainWindow.setTitle(GlobalSettings.WINDOW_TITLE + " - Powered By Java Swing");
		MainWindow.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		initWindow();
		MainWindow.setVisible(true);
		int TotalHeight = MainBox.getHeight() + MainWindow.getInsets().top;
		MainWindow.setSize(new Dimension(WINDOW_WIDTH, TotalHeight));
		MainWindow.setLocation((GlobalSettings.ScreenSize.width - MainWindow.getWidth()) / 2, (GlobalSettings.ScreenSize.height - MainWindow.getHeight()) / 2);
	}

}

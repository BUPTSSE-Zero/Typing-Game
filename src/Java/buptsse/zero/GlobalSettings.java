package buptsse.zero;

import javax.swing.*;
import javax.swing.plaf.FontUIResource;
import javax.xml.soap.Text;
import java.awt.*;
import java.util.Enumeration;

public class GlobalSettings
{
    public static Font GlobalFont = null;
    public static Dimension ScreenSize = null;

    enum SystemPlatform
    {
        OS_WINDOWS,
        OS_LINUX
    }
    public static SystemPlatform OSInfo;

    private static final float TEXT_FONT_SIZE = (float)18.0;
    private static final String ICON_PATH = "res/icon/";
    //Icons
    public static ImageIcon ICON_OPEN = null;
    public static ImageIcon ICON_EXIT = null;
    public static ImageIcon ICON_ABOUT = null;
    public static ImageIcon ICON_ENTER = null;
    public static ImageIcon ICON_OK = null;
    public static ImageIcon ICON_ERROR = null;
    public static ImageIcon ICON_PERSON = null;
    public static ImageIcon ICON_CLOCK = null;
    public static ImageIcon ICON_FINISH = null;
    public static ImageIcon ICON_START = null;
    public static ImageIcon ICON_PAUSE = null;
    public static ImageIcon ICON_QUIT = null;
    public static ImageIcon ICON_REPLAY = null;
    public static ImageIcon ICON_DIALOG_INFO = null;
    public static ImageIcon ICON_DIALOG_QUESTION = null;
    public static ImageIcon ICON_DIALOG_ERROR = null;

    //Strings
    public static String PRODUCT_NAME = "Typing Game";
    public static String WINDOW_TITLE = PRODUCT_NAME;
    public static String LABEL_PLAYER_NAME = "Player Name";
    public static String LABEL_INTERNAL_TEXT = "Internal Text(ID number)";
    public static String LABEL_EXTERNAL_TEXT = "External Text File";
    public static String LABEL_BROWSE = "Browse";
    public static String LABEL_ENTER = "Enter";
    public static String LABEL_EXIT = "Exit";
    public static String LABEL_ABOUT = "About";
    public static String LABEL_QUIT = "Quit";
    public static String LABEL_START = "Start";
    public static String LABEL_CONTINUE = "Continue";
    public static String LABEL_FINISH = "Finish";
    public static String LABEL_PAUSE = "Pause";
    public static String LABEL_REPLAY = "Replay";
    public static String MESSAGE_QUERY_EXIT = "Do you really want to exit?";
    public static String MESSAGE_PLAYER_NAME_BLANK = "The player name can't be blank.";
    public static String MESSAGE_FILE_OPEN_FAILD = "The specific text file can't be open.";
    public static String MESSAGE_TEXT_EMPTY = "Can't find any text in the specific text file.";
    public static String MESSAGE_ID_NUMBER_INVALID = "The ID number is invalid.";
    public static String MESSAGE_INPUT_ERROR = "Your input contains error(s), please check it!";
    public static String MESSAGE_CONGRATULATION = "Congratulation!";
    public static String MESSAGE_COMPLETE = "You haved completed all inputs correctly.";
    public static String MESSAGE_PLAYER_TIME = "Your time";


    public static void loadIcon()
    {
        ICON_ABOUT = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-about.png"));
        ICON_CLOCK = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-clock.png"));
        ICON_ENTER = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-enter.png"));
        ICON_EXIT = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-exit.png"));
        ICON_ERROR = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-error.png"));
        ICON_OK = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-ok.png"));
        ICON_FINISH = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-finish.png"));
        ICON_OPEN = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-open.png"));
        ICON_PAUSE = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-pause.png"));
        ICON_PERSON = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-person.png"));
        ICON_QUIT = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-quit.png"));
        ICON_REPLAY = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-replay.png"));
        ICON_START = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "icon-start.png"));
        ICON_DIALOG_INFO = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "dialog-information.png"));
        ICON_DIALOG_ERROR = new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "dialog-error.png"));
        ICON_DIALOG_QUESTION =  new ImageIcon(GlobalSettings.class.getResource(ICON_PATH + "dialog-question.png"));
    }

    //Set the UI font to the system default font and the theme to the system theme.
    public static void setUI()
    {
        ScreenSize = Toolkit.getDefaultToolkit().getScreenSize();
        try
        {
            String OSName = System.getProperty("os.name");
            if(OSName.toLowerCase().contains("windows"))
            {
                OSInfo = SystemPlatform.OS_WINDOWS;
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            }
            else
            {
                OSInfo = SystemPlatform.OS_LINUX;
                UIManager.setLookAndFeel("com.sun.java.swing.plaf.gtk.GTKLookAndFeel");
            }
        }catch(Exception e){
            e.printStackTrace();
            System.err.println("Can't load the system theme or the specific font.");
        }
        if(GlobalFont == null)
        {
            Enumeration<Object> KeyVector = UIManager.getDefaults().keys();
            while (KeyVector.hasMoreElements()) {
                Object key = KeyVector.nextElement();
                Object value = UIManager.get(key);
                if (value instanceof FontUIResource) {
                    //System.out.println("Key:" + key + " Font:" + ((Font) value).getFamily());
                    GlobalFont = ((FontUIResource) value).deriveFont(Font.PLAIN, TEXT_FONT_SIZE);
                    break;
                }
            }
        }
    }

    public static void setUIFont(String DefaultFontFamily)
    {
        //System.out.println("Default Font:" + DefaultFontFamily);
        try{
            GlobalFont = Font.decode(DefaultFontFamily).deriveFont(Font.PLAIN, TEXT_FONT_SIZE);
        }catch (Exception e){
            e.printStackTrace();
            GlobalFont = null;
        }
    }

}
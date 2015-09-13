package buptsse.zero;

import com.sun.istack.internal.NotNull;

import javax.swing.*;
import javax.swing.text.AbstractDocument;
import javax.swing.text.AttributeSet;
import javax.swing.text.PlainDocument;

public class AutoCheckDocument extends PlainDocument
{
    private ImageIcon OkIcon = null;
    private ImageIcon ErrorIcon = null;
    private String CheckText = null;
    private JLabel IndicatorLabel = null;
    private boolean CheckStatus = false;

    public boolean getCheckStatus()
    {
        return CheckStatus;
    }

    private void checkText()
    {
        boolean CheckResult = false;
        try
        {
            //System.out.println("Current Input Text:" + getText(0, getLength()));
            CheckResult = CheckText.equals(getText(0, getLength()));
        }catch (Exception e){
            e.printStackTrace();
        }
        if(CheckResult != CheckStatus)
        {
            CheckStatus = CheckResult;
            if(CheckStatus)
                IndicatorLabel.setIcon(OkIcon);
            else
                IndicatorLabel.setIcon(ErrorIcon);
        }
    }

    public AutoCheckDocument(@NotNull String CheckText,@NotNull JLabel IndicatorLabel,@NotNull ImageIcon OkIcon,@NotNull ImageIcon ErrorIcon)
    {
        this.CheckText = CheckText;
        this.IndicatorLabel = IndicatorLabel;
        this.OkIcon = OkIcon;
        this.ErrorIcon = ErrorIcon;
    }

    @Override
    protected void insertUpdate(DefaultDocumentEvent chng, AttributeSet attr) {
        super.insertUpdate(chng, attr);
        checkText();
    }

    @Override
    protected void postRemoveUpdate(DefaultDocumentEvent chng) {
        super.postRemoveUpdate(chng);
        checkText();
    }
}

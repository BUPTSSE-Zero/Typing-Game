package buptsse.zero;

import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import java.io.InputStream;
import java.util.ArrayList;

public class TextFileParser
{
    private Document doc = null;
    private NodeList TextList = null;
    public boolean parseFile(InputStream input)
    {
        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
            DocumentBuilder builder = factory.newDocumentBuilder();
            doc = builder.parse(input);
            TextList = doc.getElementsByTagName("text");
            return true;
        }catch (Exception e){
            e.printStackTrace();
            return false;
        }
    }

    public int getRowNumber()
    {
        if(doc == null || TextList == null)
            return -1;
        return TextList.getLength();
    }

    public String getRowText(int index)
    {
        String temp;
        try {
            temp = TextList.item(index).getFirstChild().getNodeValue();
        }catch (Exception e){
            e.printStackTrace();
            return null;
        }
        return temp;
    }

    public ArrayList<String> getMultiRowText()
    {
        ArrayList<String> result = new ArrayList<String>();
        String temp;
        for(int i = 0; i < getRowNumber(); i++)
        {
            temp = getRowText(i);
            //System.out.println("Row" + (i + 1) + " Text:" + temp);
            if(temp != null)
                result.add(temp);
        }
        return result;
    }
}

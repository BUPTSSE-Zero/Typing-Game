package buptsse.zero;

import org.w3c.dom.*;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.Collections;

public class RankList
{
    public static final String RANK_LIST_FILE = "RankList.xml";
    public static final int RANK_LIST_LENGTH = 10;
    private static final String ROOT_ELEMENT_NAME = "typing-game";
    private static final String RANK_LIST_ELEMENT_NAME = "rank-list";
    private static final String MD5_ATTRIBUTE = "md5";
    private static final String PLAYER_ELEMENT_NAME = "player";
    private static final String PLAYER_NAME_ATTR = "name";
    private static final String PLAYER_TIME_ATTR = "time";

    private String MD5Str = null;
    private Document doc = null;
    private Element RootNode = null;
    private Element RankListNode = null;
    public RankList()
    {
        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
            DocumentBuilder builder = factory.newDocumentBuilder();
            doc = builder.newDocument();
            doc.setXmlStandalone(true);
            doc.setXmlVersion("1.0");
            RootNode = doc.createElement(ROOT_ELEMENT_NAME);
            doc.appendChild(RootNode);
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public RankList(InputStream input) throws Exception
    {
        try {
            DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
            DocumentBuilder builder = factory.newDocumentBuilder();
            doc = builder.parse(input);
            input.close();
            RootNode = doc.getDocumentElement();
        }catch (Exception e){
            try {
                input.close();
            }catch (Exception ioe){
                throw ioe;
            }
            throw e;
        }
        if(RootNode == null || !RootNode.getNodeName().toLowerCase().equals(ROOT_ELEMENT_NAME))
        {
            System.err.println("The name of the root element of the rank list XML file is not \"" + ROOT_ELEMENT_NAME + "\"");
            throw new Exception();
        }
    }

    private String byteArrayToString(byte[] ByteArray)
    {
        StringBuffer buffer = new StringBuffer(ByteArray.length * 2 + 1);
        for(int i = 0; i < ByteArray.length; i++)
            buffer.append(Integer.toHexString(Byte.toUnsignedInt(ByteArray[i])));
        return buffer.toString();
    }

    private void createRankListNode(String TextMD5)
    {
        RankListNode = doc.createElement(RANK_LIST_ELEMENT_NAME);
        RankListNode.setAttribute(MD5_ATTRIBUTE, TextMD5);
        RootNode.appendChild(RankListNode);
    }

    public void updateRankList(ArrayList<PlayerInfo> NewPlayerList)
    {
        if(RankListNode != null)
            RootNode.removeChild(RankListNode);
        if(MD5Str == null)
            return;
        createRankListNode(MD5Str);
        for(int i = 0; i < NewPlayerList.size(); i++)
        {
            PlayerInfo player = NewPlayerList.get(i);
            Element NewPlayerNode = doc.createElement(PLAYER_ELEMENT_NAME);
            NewPlayerNode.setAttribute(PLAYER_NAME_ATTR, player.PlayerName);
            NewPlayerNode.setAttribute(PLAYER_TIME_ATTR, Long.toString(player.TypingTime));
            RankListNode.appendChild(NewPlayerNode);
        }
    }

    public ArrayList<PlayerInfo> getRankList(ArrayList<String> TextString)
    {
        if(TextString == null || TextString.size() <= 0)
            return null;
        try{
            StringBuffer TargetStr = null;
            int TotalLength = 0;
            for(int i = 0; i < TextString.size(); i++)
                TotalLength += TextString.get(i).length();
            TargetStr = new StringBuffer(TotalLength + 1);
            for(int i = 0; i < TextString.size(); i++)
                TargetStr.append(TextString.get(i));
            MessageDigest TextMD5 = MessageDigest.getInstance("MD5");
            MD5Str = byteArrayToString(TextMD5.digest(TargetStr.toString().getBytes()));
        }catch (Exception e){
            MD5Str = null;
            e.printStackTrace();
            return null;
        }
        if(!RootNode.hasChildNodes())
            return null;
        NodeList RankElementList = RootNode.getElementsByTagName(RANK_LIST_ELEMENT_NAME);
        int i;
        RankListNode = null;
        if(RankElementList == null || RankElementList.getLength() <= 0)
            return null;
        for(i = 0; i < RankElementList.getLength(); i++)
        {
            RankListNode = (Element)RankElementList.item(i);
            if(RankListNode.hasAttributes() && RankListNode.hasChildNodes())
            {
                if(RankListNode.hasAttribute(MD5_ATTRIBUTE) && RankListNode.getAttribute(MD5_ATTRIBUTE).toLowerCase().equals(MD5Str))
                {
                    ArrayList<PlayerInfo> PlayerList = new ArrayList<PlayerInfo>();
                    NodeList PlayerNodeList = RankListNode.getElementsByTagName(PLAYER_ELEMENT_NAME);
                    for(int j = 0; j < PlayerNodeList.getLength(); j++)
                    {
                        Element PlayerNode = (Element)PlayerNodeList.item(j);
                        if(PlayerNode.hasAttributes())
                        {
                            if(PlayerNode.getAttribute(PLAYER_NAME_ATTR) != null && PlayerNode.getAttribute(PLAYER_TIME_ATTR) != null)
                            {
                                try {
                                    String PlayerName = PlayerNode.getAttribute(PLAYER_NAME_ATTR);
                                    if (!GlobalSettings.checkPlayerName(PlayerName))
                                        continue;
                                    long PlayTime = Long.parseLong(PlayerNode.getAttribute(PLAYER_TIME_ATTR));
                                    if (PlayTime <= 0)
                                        continue;
                                    PlayerList.add(new PlayerInfo(PlayerName, PlayTime));
                                }catch (Exception e){
                                    e.printStackTrace();
                                    continue;
                                }
                            }
                        }
                    }
                    sortRankList(PlayerList);
                    return PlayerList;
                }
            }
        }
        RankListNode = null;
        return null;
    }

    public static void sortRankList(ArrayList<PlayerInfo> PlayerList)
    {
        Collections.sort(PlayerList);
        if(PlayerList.size() > RANK_LIST_LENGTH)
        {
            for(int i = RANK_LIST_LENGTH; i < PlayerList.size(); i++)
            {
                if(PlayerList.get(i).compareTo(PlayerList.get(RANK_LIST_LENGTH - 1)) != 0)
                {
                    while (PlayerList.size() > i)
                        PlayerList.remove(i);
                    break;
                }
            }
        }
    }

    public void writeRankListFile()
    {
        Transformer transformer = null;
        try{
            transformer = TransformerFactory.newInstance().newTransformer();
        }catch (Exception e){
            e.printStackTrace();
            return;
        }
        transformer.setOutputProperty(OutputKeys.ENCODING, "utf-8");
        transformer.setOutputProperty(OutputKeys.VERSION, "1.0");
        try {
            DOMSource DOMSrc = new DOMSource(doc);
            StreamResult DOMStream = new StreamResult(new FileOutputStream(RANK_LIST_FILE));
            transformer.transform(DOMSrc, DOMStream);
        }catch (Exception e){
            e.printStackTrace();
            return;
        }
    }
}

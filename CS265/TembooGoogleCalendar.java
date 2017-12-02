import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.Scanner;

import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonObject;
import com.google.gson.JsonParser;
import com.temboo.Library.Google.Calendar.GetAllCalendars;
import com.temboo.Library.Google.Calendar.GetAllCalendars.GetAllCalendarsInputSet;
import com.temboo.Library.Google.Calendar.GetAllCalendars.GetAllCalendarsResultSet;
import com.temboo.core.TembooException;
import com.temboo.core.TembooSession;


public class TembooGoogleCalendar {
	// http://www.xyzws.com/Javafaq/how-to-use-httpurlconnection-post-data-to-web-server/139
	 public static String executePost(String targetURL, String urlParameters)
	  {
	    URL url;
	    HttpURLConnection connection = null;  
	    try {
	      //Create connection
	      url = new URL(targetURL);
	      connection = (HttpURLConnection)url.openConnection();
	      connection.setRequestMethod("POST");
	      connection.setRequestProperty("Content-Type", 
	           "application/x-www-form-urlencoded");
				
	      connection.setRequestProperty("Content-Length", "" + 
	               Integer.toString(urlParameters.getBytes().length));
	      connection.setRequestProperty("Content-Language", "en-US");  
				
	      connection.setUseCaches (false);
	      connection.setDoInput(true);
	      connection.setDoOutput(true);

	      //Send request
	      DataOutputStream wr = new DataOutputStream (
	                  connection.getOutputStream ());
	      wr.writeBytes (urlParameters);
	      wr.flush ();
	      wr.close ();

	      //Get Response	
	      InputStream is = connection.getInputStream();
	      BufferedReader rd = new BufferedReader(new InputStreamReader(is));
	      String line;
	      StringBuffer response = new StringBuffer(); 
	      while((line = rd.readLine()) != null) {
	        response.append(line);
	        response.append('\r');
	      }
	      rd.close();
	      return response.toString();

	    } catch (Exception e) {

	      e.printStackTrace();
	      return null;

	    } finally {

	      if(connection != null) {
	        connection.disconnect(); 
	      }
	    }
	  }
	
	/**
	 * @param args
	 * @throws TembooException 
	 */
	public static void main(String[] args) throws TembooException {
		String acctName, appKeyName, appKeyValue;
		String client_id, redirect_uri, code, client_secret, access_token;
		
		Scanner in = new Scanner(System.in);
		
		System.out.println("Enter Temboo Account Name");
		acctName = in.nextLine();
		System.out.println("Enter Temboo App Key Name");
		appKeyName = in.nextLine();
		System.out.println("Enter Temboo App Key Value");
		appKeyValue = in.nextLine();		
		
		System.out.println("Enter Google Client ID");
		client_id = in.nextLine();

		System.out.println("Enter Google Client Secret");
		client_secret = in.nextLine();

		System.out.println("Enter Google Redirect URI");
		redirect_uri = in.nextLine();
	
		// https://developers.google.com/accounts/docs/OAuth2WebServer#offline
		String oauthURL = "https://accounts.google.com/o/oauth2/auth?access_type=offline&client_id=" + client_id + "&scope=https://www.googleapis.com/auth/calendar&response_type=code&redirect_uri=" + redirect_uri + "&state=/profile&approval_prompt=force";
		System.out.println("Go to the following URL and obtain the code that you find there.\n" + oauthURL);
		code = in.nextLine();
		
		// Google requires a POST for the next step
		String authorizeURL = "https://accounts.google.com/o/oauth2/token";
		String authorizeParams = "code=" + code + "&client_id=" + client_id + "&client_secret=" + client_secret + "&redirect_uri=" + redirect_uri + "&grant_type=authorization_code";
		String authorizeResponse = executePost(authorizeURL, authorizeParams); 
		
		// Since the response is json, convert to a JSON object to obtain oauth token
		// We could parse it ourselves, but why would we do that? :-)
    	JsonParser oauth_jp = new JsonParser();
    	JsonElement oauth_root = oauth_jp.parse(authorizeResponse);
    	JsonObject oauth_rootobj = oauth_root.getAsJsonObject(); // may be Json Array if it's an array, or other type if a primitive
    	access_token = oauth_rootobj.get("access_token").getAsString();
		System.out.println("Got oauth access token: " + access_token);
		
		// It's temboo time
		TembooSession session = new TembooSession(acctName, appKeyName, appKeyValue);
		GetAllCalendars getAllCalendarsChoreo = new GetAllCalendars(session);

		// Get an InputSet object for the choreo
		GetAllCalendarsInputSet getAllCalendarsInputs = getAllCalendarsChoreo.newInputSet();

		// Set inputs
		getAllCalendarsInputs.set_ClientSecret(client_secret);
		getAllCalendarsInputs.set_AccessToken(access_token);
		getAllCalendarsInputs.set_ClientID(client_id);

		// Execute Choreo
		GetAllCalendarsResultSet getAllCalendarsResults = getAllCalendarsChoreo.execute(getAllCalendarsInputs);
		
		System.out.println(getAllCalendarsResults.get_Response());
		
		// Now parse the json
    	JsonParser jp = new JsonParser();
    	JsonElement root = jp.parse(getAllCalendarsResults.get_Response());
    	JsonObject rootobj = root.getAsJsonObject(); // may be Json Array if it's an array, or other type if a primitive
    	
    	JsonArray items = rootobj.get("items").getAsJsonArray();
    	for(int i = 0; i < items.size(); i++) {
    		JsonObject item = items.get(i).getAsJsonObject();
    		String id = item.get("id").getAsString();
    		System.out.println("Found calendar id: " + id);
    	}
	}

}

# call_api.py
import requests
import json
import sys

def call_gemini_api(prompt):
    API_KEY = "AIzaSyBdNrevCJuKAvP5yFgXleU_rQOzzLQen0M"
    url = f"https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key={API_KEY}"
    
    payload = {
        "contents": [{
            "parts": [{"text": prompt}]
        }]
    }
    
    headers = {
        "Content-Type": "application/json"
    }
    
    try:
        response = requests.post(url, headers=headers, json=payload)
        response.raise_for_status()  # Raise an exception for 4XX/5XX responses
        
        data = response.json()
        
        # Extract the answer from the response
        try:
            answer = data["candidates"][0]["content"]["parts"][0]["text"]
            return answer
        except (KeyError, IndexError) as e:
            print(f"Error extracting response: {e}")
            print("Unexpected response structure:", json.dumps(data, indent=2))
            return None
            
    except requests.exceptions.RequestException as e:
        print(f"API request failed: {e}")
        if hasattr(e, 'response') and e.response:
            print(f"Response status: {e.response.status_code}")
            print(f"Response body: {e.response.text}")
        return None

if __name__ == "__main__":
    # Default prompt or get from command line arguments
    prompt = "write code c++ to print hello world with username"
    
    if len(sys.argv) > 1:
        prompt = sys.argv[1]
    
    result = call_gemini_api(prompt)
    
    if result:
        print(result)
    else:
        print("Failed to get a response from the Gemini API")
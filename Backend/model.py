import requests
import json
import os
import google.generativeai as genai


genai.configure(api_key="AIzaSyCw_Nfdw1CHnFXqPEqHC66yWItOvQRKrHc")


# Create the model
generation_config = {
  "temperature": 1,
  "top_p": 0.95,
  "top_k": 40,
  "max_output_tokens": 8192,
  "response_mime_type": "text/plain",
}

model = genai.GenerativeModel(
  model_name="gemini-1.5-pro-002",
  generation_config=generation_config,
  # safety_settings = Adjust safety settings
  # See https://ai.google.dev/gemini-api/docs/safety-settings
)

chat_session = model.start_chat(
  history=[
    {
      "role": "user",
      "parts": [
        "your product description expert and your  role is to generate json format \"  {\n        product_name :str,\n        brand_name:str,\n        product_count: integer,\n        product_quantity:float\n        is_expired:boolean,\nexpiry_date:number of days left(integer)\n    }\"  based on the data",
      ],
    },
    {
      "role": "model",
      "parts": [
        "Okay, I'm ready. Please provide the data you want me to convert into JSON format.  I'll create a JSON object for each product based on the data you give me.  Make sure your data includes the necessary information for each field (product name, brand name, product count, product quantity, expiry information).\n\nFor example, you can provide data like this:\n\n\"Product A, Brand X, 10 boxes, 2.5 kg, Expires in 30 days\"\n\nOr, if some products don't have expiry dates:\n\n\"Product B, Brand Y, 5 bottles, 1 liter, No expiry\"\n\nI will then convert it to JSON like this (for the first example):\n\n```json\n{\n  \"product_name\": \"Product A\",\n  \"brand_name\": \"Brand X\",\n  \"product_count\": 10,\n  \"product_quantity\": 2.5,\n  \"is_expired\": false,\n  \"expiry_date\": 30\n}\n```\n\nAnd for the second example:\n\n```json\n{\n  \"product_name\": \"Product B\",\n  \"brand_name\": \"Brand Y\",\n  \"product_count\": 5,\n  \"product_quantity\": 1.0,\n  \"is_expired\": false,\n  \"expiry_date\": null \n}\n```\nI'll use `null` for `expiry_date` if there's no expiry information.  If a product *is* expired, I'll set `is_expired` to `true` and `expiry_date` to a negative number representing the number of days *past* the expiry date.\n",
      ],
    },
  ]
)


def lookup_product(barcode):
    #api_key = 'YOUR_API_KEY'  # Replace this with your API key
    url = f"https://api.upcitemdb.com/prod/trial/lookup?upc={barcode}"  # UPCitemdb API URL

    headers = {
        "Content-Type": "application/json",
        "Accept": "application/json",
        
    }

    try:
        response = requests.get(url, headers=headers)
        if response.status_code == 200:
            product_data = response.json()
            print(json.dumps(product_data, indent=4))  # Print the entire JSON response for inspection
            # Extract and return product details (modify based on the API response structure)
            if 'items' in product_data and len(product_data['items']) > 0:
                product = product_data['items'][0]
                data = {
                    'Title': product.get('title', 'N/A'),
                    'Brand': product.get('brand', 'N/A'),
                    'Category': product.get('category', 'N/A'),
                    'Description': product.get('description', 'N/A')
                }
                return data

            
            else:
                return None
        else:
            print(f"Error: API request failed with status code {response.status_code}")
            return None
    except Exception as e:
        print(f"Error in product lookup: {e}")
        return None


def main():
    barcode = input("Enter a product barcode: ")
    product_data = lookup_product(barcode)
    if product_data:
        print("\nProduct details:")
        print(f"Title: {product_data['Title']}")
        print(f"Brand: {product_data['Brand']}")
        print(f"Category: {product_data['Category']}")
        print(f"Description: {product_data['Description']}")
    else:
        print("Product not found or details unavailable.")


if __name__ =="__main__":
   main()
   
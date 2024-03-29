# Using SendGrid and cURL

## Sending email from template
```
curl --request POST \
  --url https://api.sendgrid.com/v3/mail/send \
  --header 'authorization: Bearer YOUR_API_KEY' \
  --header 'Content-Type: application/json' \
  --data '{"personalizations": [{"to": [{"email": "recipient@example.com"}]}],"from": {"email": "sendeexampexample@example.com"},"subject":"Hello, World!","content": [{"type": "text/plain","value": "Heya!"}], "template_id" : "YOUR_TEMPLATE_ID"}'
```
## Sending email at a Scheduled Time

### Option 1: Sending a Basic Email
```
curl --request POST \
  --url https://api.sendgrid.com/v3/mail/send \
  --header 'authorization: Bearer YOUR_API_KEY' \
  --header 'Content-Type: application/json' \
  --data '{"personalizations": [{"to": [{"email": "recipient@example.com"}]}],"from": {"email": "sendeexampexample@example.com"},"subject":"Hello, World!","content": [{"type": "text/plain","value": "Heya!"}], "send_at" : UNIX_TIMESTAMP_HERE}'
```

### Option 2: Scheduling and Cancelling an Email

Step 1: Generate a batch ID
```
curl --request POST \
  --url https://api.sendgrid.com/v3/mail/batch \
  --header 'authorization: Bearer YOUR_API_KEY' \
  --header 'Content-Type: application/json' \
```

Step 2: Schedule the email to be sent, using your new batch ID
```
curl --request POST \
  --url https://api.sendgrid.com/v3/mail/send \
  --header 'authorization: Bearer YOUR_API_KEY' \
  --header 'Content-Type: application/json' \
  --data '{"personalizations": [{"to": [{"email": "recipient@example.com"}]}],"from": {"email": "sendeexampexample@example.com"},"subject":"Hello, World!","content": [{"type": "text/plain","value": "Heya!"}], "send_at" : UNIX_TIMESTAMP_HERE, "batch_id" : "YOUR_BATCH_ID"}'
```

Step 3: Cancel the scheduled email
```
curl --request POST \
  --url https://api.sendgrid.com/v3/user/scheduled_sends \
  --header 'authorization: Bearer YOUR_API_KEY' \
  --header 'Content-Type: application/json' \
  --data '{"batch_id":"YOUR_BATCH_ID","status":"cancel"}'
```

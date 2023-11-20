//var bird = require('messagebird')('65z4Ntn8Z7DzMLdPGaVv91unx');
const bird = require('messagebird').initClient('65z4Ntn8Z7DzMLdPGaVv91unx');
const originator = 'YourName'; // Check if the originator is allowed by the SMS provider
const recipients = [8800117410]; // Ensure the phone number is valid
const messageContent = 'Hello, this is a test message from MessageBird!'; // Ensure the message content is within limits

bird.messages.create(
  {
    to: recipients,
    originator: originator,
    body: messageContent, // Use 'body' instead of 'message' for the message content
  },
  (error, response) => {
    if (error) {
      console.error(`Error sending message: ${error.message}`);
    } else {
      console.log(`Message ID: ${response.id}`);
    }
  }
);

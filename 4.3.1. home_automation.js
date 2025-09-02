import express from "express";
import fetch from "node-fetch";

const app = express();
app.use(express.json());

app.post("/command", async (req, res) => {
  const userInput = req.body.message;

  // Call LLM API (OpenAI/Gemini)
  const response = await fetch("https://api.openai.com/v1/chat/completions", {
    method: "POST",
    headers: {
      "Authorization": `Bearer YOUR_API_KEY`,
      "Content-Type": "application/json"
    },
    body: JSON.stringify({
      model: "gpt-4o-mini",  // or Gemini model
      messages: [
        { role: "system", content: "You are a home automation assistant. Extract commands in JSON." },
        { role: "user", content: userInput }
      ],
      temperature: 0
    })
  });

  const data = await response.json();
  const command = data.choices[0].message.content;

  // Example output: {"device":"light","room":"living","action":"on"}
  console.log("Command:", command);

  // Forward to ESP32 via HTTP/MQTT
  await fetch("http://ESP32_IP/execute", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: command
  });

  res.json({ success: true, command });
});

app.listen(3000, () => console.log("Server running on port 3000"));

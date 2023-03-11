const submitButton = document.getElementById("submit");
const cityInput = document.getElementById("city");
const locationText = document.getElementById("location");
const temperatureText = document.getElementById("temperature");
const descriptionText = document.getElementById("description");
const weatherInfo = document.querySelector(".weather-info");

submitButton.addEventListener("click", () => {
  const city = cityInput.value;
  const apiKey = "81a92e33853cee3579c39858c4a07aa1"; // Replace with your own API key
  const apiUrl = `https://api.openweathermap.org/data/2.5/weather?q=${city}&appid=${apiKey}&units=metric`;

  fetch(apiUrl)
    .then((response) => response.json())
    .then((data) => {
      const location = data.name + ", " + data.sys.country;
      const temperature = data.main.temp + " °C";
      const description = data.weather[0].description;

      locationText.innerText = location;
      temperatureText.innerText = temperature;
      descriptionText.innerText = description;

      weatherInfo.style.display = "block";
    })
    .catch((error) => {
      console.error(error);
      alert("Error getting weather data. Please try again.");
    });
});

const apiKey = "81a92e33853cee3579c39858c4a07aa1";
const apiUrl = "https://api.openweathermap.org/data/2.5/forecast";
const form = document.querySelector("form");
const forecastContainer = document.getElementById("forecast");

form.addEventListener("submit", (e) => {
  e.preventDefault();
  const city = e.target.elements.city.value;
  const params = {
    q: city,
    appid: apiKey,
    units: "metric",
  };
  const urlParams = new URLSearchParams(params);
  const requestUrl = `${apiUrl}?${urlParams.toString()}`;

  fetch(requestUrl)
    .then((response) => response.json())
    .then((data) => {
      console.log(data);
      forecastContainer.innerHTML = "";
      data.list.forEach((item) => {
        const listItem = document.createElement("div");
        listItem.classList.add("forecast-item");
        const dateTime = new Date(item.dt_txt);
        const date = `${dateTime.toDateString()} - ${dateTime.toLocaleTimeString()}`;
        const rain = item.rain ? `${item.rain["3h"]} mm` : "0 mm";
        listItem.innerHTML = `
          <h3>${date}</h3>
          <p>Temperature: ${item.main.temp} °C</p>
          <p>Humidity: ${item.main.humidity} %</p>
          <p>Rainfall: ${rain}</p>
        `;
        forecastContainer.appendChild(listItem);
      });
    })
    .catch((error) => {
      console.error(error);
      forecastContainer.innerHTML = `<p>Something went wrong. Please try again later.</p>`;
    });
});

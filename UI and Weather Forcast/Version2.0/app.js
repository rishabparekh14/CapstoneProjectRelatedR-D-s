const apiKey = "81a92e33853cee3579c39858c4a07aa1";
const apiUrl = `https://api.openweathermap.org/data/2.5/weather?q=hayward&appid=${apiKey}&units=metric`;

const city = document.getElementById("city");
const country = document.getElementById("country");
const temp = document.getElementById("temp");
const description = document.getElementById("description");
const dataContainer = document.getElementById("data");

fetch(apiUrl)
  .then((response) => response.json())
  .then((data) => {
    console.log(data);
    city.innerText = data.name;
    country.innerText = data.sys.country;
    temp.innerText = data.main.temp + "C";
    description.innerText = data.weather[0].description;

    const dataKeys = Object.keys(data);
    const dataList = document.createElement("ul");
    dataKeys.forEach((key) => {
      const listItem = document.createElement("li");
      listItem.innerText = `${key}: ${JSON.stringify(data[key])}`;
      dataList.appendChild(listItem);
    });
    dataContainer.appendChild(dataList);
  });

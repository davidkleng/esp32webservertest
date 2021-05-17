const DELAY_READ_ADC = 10000
let led1State = 0
let led2State = 0
let led3State = 0

let isSlideACtive = false

function initStates() {

  $.ajax({
    type: "GET",
    url: "/states",
    success: function (value) {
      console.log(value)
        const slider = document.getElementById("slider-range")
        const gaugeElement = document.querySelector(".gauge")
      const values = value.split(",")
      led1State = parseInt(values[0])
      led2State = parseInt(values[1])
      led3State = parseInt(values[2])
      const output = document.getElementById("pwm-value")
      output.innerHTML = parseInt(values[3])
      slider.value = values[3]
      if (parseFloat(values[4])>0){
        setGaugeValue(gaugeElement, parseFloat(values[4]) / 3.3)
      }
      if (led1State === 1) {
        led1.style = "background-color:#47a04f"
      } else {
        led1.style = "background-color:#ff8080"
      }
      if (led2State === 1) {
        led2.style = "background-color:#47a04f"
      } else {
        led2.style = "background-color:#ff8080"
      }
      if (led3State === 1) {
        led3.style = "background-color:#47a04f"
      } else {
        led3.style = "background-color:#ff8080"
      }
    },
    error: function () {
      console.log("No se ha podido obtener los estados iniciales")
    },
  })
}

function getModbusValues() {
  const modbusList = document.getElementById("modbus-list")
  const id = document.getElementById("id")
  const address = document.getElementById("address")
  const quantity = document.getElementById("quantity")
  // console.log(id.value,address.value,quantity.value);
  $.get(
    "/modbus",
    {
      id: id.value,
      address: address.value,
      quantity: quantity.value,
    },
    (response) => {
      console.log(response)
      if (response !== "ERROR") {
        listElementsModbus(id.value, address.value, response)
      } else {
        alert("Modbus no disponible")
      }
    }
  )
  // id.value=''
  // address.value=''
  // quantity.value=''
  // const modbusList = document.getElementById("modbus-list")
  // modbusList.innerHTML = ''
}

function listElementsModbus(id, address, stringValues) {
  const modbusList = document.getElementById("modbus-list")

  let values = stringValues.split(",")

  for (let i = 0; i < values.length; i++) {
    modbusList.innerHTML += `<li> 
    ID: ${id} -
    Dirección: ${parseInt(address) + i} -
    ${values[i]}
    </li>`
  }
}

/* Toggle between adding and removing the "responsive" class to topnav when the user clicks on the icon */
function responsiveNavbar() {
  const x = document.getElementById("myTopnav")
  if (x.className === "topnav") {
    x.className += " responsive"
  } else {
    x.className = "topnav"
  }
}

async function sendLed1State(event) {
  led1State === 0 ? (led1State = 1) : (led1State = 0)
  if (led1State === 1) {
    led1.style = "background-color:#47a04f"
  } else {
    led1.style = "background-color:#ff8080"
  }
  $.ajax({
    type: "POST",
    url: "/led1",
    data: {
      led1: led1State,
    },
    dataType: "json",
  })

  
}

async function sendLed2State(event) {
  led2State === 0 ? (led2State = 1) : (led2State = 0)
  if (led2State === 1) {
    led2.style = "background-color:#47a04f"
  } else {
    led2.style = "background-color:#ff8080"
  }
  $.ajax({
    type: "POST",
    url: "/led2",
    data: {
      led2: led2State,
    },
    dataType: "json",
  })

  
}
async function sendLed3State(event) {
  led3State === 0 ? (led3State = 1) : (led3State = 0)
  if (led3State === 1) {
    led3.style = "background-color:#47a04f"
  } else {
    led3.style = "background-color:#ff8080"
  }  
  $.ajax({
    type: "POST",
    url: "/led3",
    data: {
      led3: led3State,
    },
    dataType: "json",
  })
  
}


// Update the current slider value (each time you drag the slider handle)
async function sendSliderValue() {
  if (!isSlideACtive) {
    isSlideACtive = true
    const output = document.getElementById("pwm-value")
    const slider = document.getElementById("slider-range")
    output.innerHTML = slider.value
    console.log(slider.value)

    $.ajax({
      type: "POST",
      url: "/pwm",
      data: {
        pwm: slider.value,
      },
      dataType: "json",
    })
    setTimeout(() => {
      isSlideACtive = false
    }, 300)
  }
}

// gauge

function setGaugeValue(gauge, value) {
  if (value < 0 || value > 1) {
    return
  }

  gauge.querySelector(".gauge__fill").style.transform = `rotate(${
    value / 2
  }turn)`
  gauge.querySelector(".gauge__cover").textContent = `${Math.round(
    value * 100
  )}%`
}

function getAdcValue() {
  // pensar como borrar
  const gaugeElement = document.querySelector(".gauge")
  // pensar como borrar
  $.ajax({
    type: "GET",
    url: "/adc",
    success: function (value) {
      console.log(parseFloat(value))
      if (parseFloat(value)>0){
        setGaugeValue(gaugeElement, parseFloat(value) / 3.3)
      }
      setTimeout(getAdcValue, DELAY_READ_ADC)
    
    },
    error: function () {
      console.log("No se ha podido obtener la información")
    },
  })
}

function onLogout() {
  $.ajax({
    type: "GET",
    url: "/logout",
    success: function (respuesta) {
      console.log(respuesta)
    },
    error: function () {
      console.log("No se ha podido cerrar la sesión")
    },
  })
}

window.onload = () => {
  const led1 = document.getElementById("led1")
  const led2 = document.getElementById("led2")
  const led3 = document.getElementById("led3")

  const slider = document.getElementById("slider-range")
  const output = document.getElementById("pwm-value")

  const gaugeElement = document.querySelector(".gauge")

  const modbusForm = document.querySelector("form")

  //router
  const wrap = document.getElementById("myTopnav")
  wrap.addEventListener("click", (event) => {
    // Check if it’s the list element, we want the clicks only from them
    if (event.target.classList.contains("topnav--item")) {     
  
      //delete active class from all elements of navbar
      Array.from(document.querySelectorAll(".active")).forEach(function (
        el
      ) {
        el.classList.remove("active")
      })
      console.log(event.target);
      //set active class only to the selected item
      event.target.classList.add("active");
    }
  })
  //router


  modbusForm.addEventListener("submit", (event) => {
    event.preventDefault()
    // actual logic, e.g. validate the form
    // getAdcValue()
    // listElementsModbus()
    getModbusValues()
  })
  //!testear
  initStates()
  //!testear
  output.innerHTML = slider.value // Display the default slider value

  led1.onclick = sendLed1State
  led2.onclick = sendLed2State
  led3.onclick = sendLed3State

  // modbusButtton.onclick=getAdcValue

  slider.oninput = sendSliderValue
  setGaugeValue(gaugeElement, 0)

  // schedule the first invocation:
  setTimeout(getAdcValue, DELAY_READ_ADC)
}

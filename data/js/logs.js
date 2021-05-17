/* Toggle between adding and removing the "responsive" class to topnav when the user clicks on the icon */
function responsiveNavbar() {
  const x = document.getElementById("myTopnav")
  if (x.className === "topnav") {
    x.className += " responsive"
  } else {
    x.className = "topnav"
  }
}

function getFilesSD() {
  //   const filesList = document.getElementById("files--list")

  $.get("/files", (response) => {
    console.log(response)
    if (response !== "ERROR") {
      listFilesSD(response)
    } else {
      alert("Files no disponible")
    }
  })
}

function listFilesSD(stringValues) {
  const filesList = document.getElementById("files--list")

  let values = stringValues.split(",")

  for (let i = 0; i < values.length; i++) {
    filesList.innerHTML += `<li>   
    ${values[i]}
    </li>`
  }
}

window.onload = () => {
  //router
  const wrap = document.getElementById("myTopnav")
  wrap.addEventListener("click", (event) => {
    // Check if it’s the list element, we want the clicks only from them
    if (event.target.classList.contains("topnav--item")) {
      //delete active class from all elements of navbar
      Array.from(document.querySelectorAll(".active")).forEach(function (el) {
        el.classList.remove("active")
      })
      console.log(event.target)
      //set active class only to the selected item
      event.target.classList.add("active")
    }
  })
  //router

  getFilesSD()
}

// const btn = document.getElementById('st-btn1');

// btn.addEventListener('click', function onClick() {
//   btn.style.backgroundColor = 'rgb(117, 214, 117)';
//   btn.style.color = 'white';
//   btn.innerText='Done'
// });

function myfunction(val){
    const btn = document.getElementById('st-btn'+val);
    btn.style.backgroundColor = 'rgb(117, 214, 117)';
    btn.style.color = 'white';
    btn.innerText='Done';
}
let progressBar1 = document.querySelector(".cp1");
let valueContainer1 = document.querySelector(".vc1");

let progressValue1 = 0;
let progressEndValue1 = 65;
let speed = 40;

valueContainer1.style.fontSize="30px"
let progress1 = setInterval(() => {
    progressValue1++;
    var res=`${progressValue1}%`+'\n'+'Pending';
    valueContainer1.textContent = res;
  progressBar1.style.background = `conic-gradient(
      #e62929 ${progressValue1 * 3.6}deg,
      #ecedf2 ${progressValue1 * 3.6}deg
  )`;
  if (progressValue1 == progressEndValue1) {
    clearInterval(progress1);
  }
}, speed);

// for progress bar-2

let progressBar2 = document.querySelector(".cp2");
let valueContainer2 = document.querySelector(".vc2");

let progressValue2 = 0;
let progressEndValue2 = 35;

valueContainer2.style.fontSize="30px"
let progress2 = setInterval(() => {
    progressValue2++;
    valueContainer2.textContent = `${progressValue2}%      
    Done`;
    // valueContainer2.textContent = `Done`;
  progressBar2.style.background = `conic-gradient(
      #46b62a ${progressValue2 * 3.6}deg,
      #ecedf2 ${progressValue2 * 3.6}deg
  )`;
  if (progressValue2 == progressEndValue2) {
    clearInterval(progress2);
  }
}, speed);

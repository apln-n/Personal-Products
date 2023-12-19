
const height = 200;
// const width = height;

// https://developer.mozilla.org/ja/docs/Web/API/FileReader/readAsText
const showHrefs = () => {
    const p = document.querySelector("p.inner");
    const [file] = document.querySelector('input[type=file]').files;
    const reader = new FileReader();
    // innerHTMLで読み込み  
    reader.addEventListener("load", () => {
      p.innerHTML = reader.result;
      showIframeHref();
    }, false);
    //fileは外側に記述する
    if (file) {
      reader.readAsText(file);
    }
}

const showIframeHref = () => {
    const myDiv = (as_a) => {
        const div = document.createElement("div");
        div.className = "myDiv";
        //この形にしないとループ処理で大変なことになる
        const a = as_a.cloneNode(true);
        a.target = "_blank"
        a.textContent = a.textContent.substring(0,a.textContent.length-"- Yahoo!知恵袋".length)
        const img = document.createElement("img");
        const question = a.href.substring("https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q".length, "https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q12345678901".length)
        img.src = "https://chie-pctr.c.yimg.jp/dk/iwiz-chie/que-"+question+"?&h="+height+"&up=0"
        if(img.src != ""){
            img.style = "display: inline-block;display:table-cell;vertical-align:middle;text-align: center;margin: 0 auto";
            a.style = img.style;
            div.appendChild(img);
            div.appendChild(a);
            div.style = "flex: auto;display:table;width: 18%; margin: 5px";
            return div;
        }else{
            return null;
        }
    }
    const p = document.querySelector("p.href");
    const as = document.querySelector("p.inner").getElementsByTagName("a")
    //なぜかfor a in asだとダメ
    p.textContent = "";
    let count=0, i = 0;
    const maxIndex = as.length;
    console.log("length = "+as.length)
    //aをappendChildするたびにas.lengthが減るので、単純なforループなどを使えない仕様
    for(let i=0;i<as.length;i++){
        console.log("i = " + i)
        if( as[i].href.includes("https://detail.chiebukuro.yahoo.co.jp") ){
            //myDivで一度aを複製してからappendしている、そうしないとforループのiがずれる
            const div = myDiv(as[i]);
            if(div != null){
                p.appendChild(div);
                count++;
            }
            //保険
            if(count >= maxIndex){
                break;
            }
        }
    }
    console.log("count = "+count);
}

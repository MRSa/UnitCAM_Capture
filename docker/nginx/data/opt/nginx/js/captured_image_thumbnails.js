document.addEventListener('DOMContentLoaded', function() {
    const thumbnailsDiv = document.getElementById('thumbnails');


    var pathName = window.location.pathname;
    const targetPathName = pathName.substring(0, pathName.lastIndexOf('/') + 1);
    console.log('Location:', targetPathName);
    fetch(targetPathName)
        //.then(response => response.text())
        .then(response => {
            console.log('Response:', response);
            return (response.text());
        })
        .then(html => {
            console.log('HTML:', html);
            const parser = new DOMParser();
            const doc = parser.parseFromString(html, 'text/html');
            const links = doc.querySelectorAll('a');
            console.log('Links:', links);

            links.forEach(link => {
                const href = link.getAttribute('href');
                if (href && href.toLowerCase().endsWith('.jpg')) {
                    const img = document.createElement('img');
                    // 相対パスを絶対パスに変換
                    const absoluteHref = new URL(href, window.location.href).href;
                    img.src = absoluteHref;
                    img.className = 'thumbnail';
                    thumbnailsDiv.appendChild(img);
                }
            });
        });
    console.log('finished.');
});


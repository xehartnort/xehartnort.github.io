
# Buscadores

### Google
<div markdown = "0">
    <form id = "searchform" target = "_blank" method = "get" action = "https://www.google.com/search">
        <input name = "q" type = "text" maxlength="255" placeholder = "Prueba buscando: algoritmica 1112. Powered by Google" style="width:100%;height:40px;padding:12px 40px 12px 12px">
        <input value = "xehartnort.github.io" type = "hidden" name = "sitesearch" >
    </form>
</div>


### Duckduckgo
<div markdown = "0">
    <form id = "searchform" target = "_blank" method = "get" action = "https://duckduckgo.com">
        <input name = "q" type = "text" maxlength="255" placeholder = "Prueba buscando: algoritmica 1112. Powered by Duckduckgo" style="width:100%;height:40px;padding:12px 40px 12px 12px">
        <input value = "xehartnort.github.io" type = "hidden" name = "site" >
    </form>
</div>

# ¿Cómo funciona?

Este projecto se sirve del servicio de indexación de Google para realizar búsquedas sobre la estructura de directorios. En el futuro, nos gustaría ser compatibles con más buscadores, tales como Duckduckgo y Bing.

# ¿Cómo contribuir?

Para contribuir crea un Pull Request en el que añadas los documentos en la carpeta correspondiente de la estructura de directorios. Cuándo dicho Pull Request sea aceptado, se añadiran los nuevos documentos al sitemap de la web y en breves, será indexado por Google.

### Estructura de los directorios

Cada archivo se encuentra almacenado en las profundidades de la estructura de directorios atendiendo a los siguientes criterios: grado al que pertenece el examen, curso de la asignatura del examen, asignatura del examen y año del curso en el que se realiza el examen. En caso de no poder determinar el año o ser este anterior a 2011-2012, el valor de año es UNKN (de Unknown, desconocido) o ANTE (de Anterior al origen del mundo al año 1112).

Por ejemplo, si tenemos un **examen.pdf** del curso 1º, año 2013-2014, asignatura **Héchizos y Pócimas** del grado en *Artes Oscuras* y un **tipo_test.pdf** de dicha asígnatura que no sabemos de que año es, pero tienen hasta telarañas, sus rutas en la estructura serían:

- exámenes/Artes Oscuras/1/Héchizos y Pócimas/1314/examen.pdf
- exámenes/Artes Oscuras/1/Héchizos y Pócimas/UNKN/tipo_test.pdf

# FAQ

Consulta la web de la https://deiit.ugr.es/BDExamenes

import datetime

def detectar_tipos_desde_datos(lista_registros):
    if not lista_registros:
        raise ValueError("No hay registros para analizar.")

    tipos = {}
    for campo in lista_registros[0].keys():
        max_len = 0
        es_int = es_float = es_bool = es_date = True

        for fila in lista_registros:
            val = fila[campo]
            if isinstance(val, str):
                max_len = max(max_len, len(val))
                if val.lower() not in ("true", "false", "0", "1"):
                    es_bool = False
                try:
                    int(val)
                except: es_int = False
                try:
                    float(val)
                except: es_float = False
                try:
                    datetime.date.fromisoformat(val)
                except: es_date = False
            elif isinstance(val, bool):
                es_float = es_int = es_date = False
            elif isinstance(val, int):
                es_float = es_date = False
            elif isinstance(val, float):
                es_date = False
            else:
                es_int = es_float = es_bool = es_date = False

        if es_bool: tipos[campo] = "bool"
        elif es_int: tipos[campo] = "int"
        elif es_float: tipos[campo] = "double"
        elif es_date: tipos[campo] = "date"
        else: tipos[campo] = f"char:{max_len}"

    return tipos

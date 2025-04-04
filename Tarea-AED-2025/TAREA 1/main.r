library(ggplot2)

mis_resultados <- data.frame(
    Metodo = rep(c("Directo", "Polimorfismo", "PunteroFuncion", "Functor"), each = 5),
    Tamanio = rep(c(1000, 2500, 5000, 7500, 10000), times = 4),
    Tiempo_ms = c(
        # Directo 
        5.2, 336, 866.6, 1961,3492.2,
        
        # Polimorfismo 
        5, 317.2, 910.2, 1940.4, 3456,
        
        # PunteroFuncion 
        6.6, 389.2, 1018, 2321, 4066,
        
        # Functor 
        5.2, 317, 873.4, 1943.2, 3431.4
    )
)

colores_primarios <- c(
    "Directo" = "#0000FF", 
    "Polimorfismo" = "#FF0000", 
    "PunteroFuncion" = "#00AA00", 
    "Functor" = "#FFD700" 
)

    ggplot(mis_resultados, aes(x = Tamanio, y = Tiempo_ms, color = Metodo)) +
    geom_point(size = 3, alpha = 0.8) +
    geom_smooth(method = "loess", span = 0.7, se = FALSE, linewidth = 0.7) + 
    labs(
        title = "Comparación de Rendimiento entre Implementaciones",
        x = "Tamaño del Vector (elementos)",
        y = "Tiempo de Ejecución (milisegundos)",
        color = "Método:"
    ) +
    scale_color_manual(values = colores_primarios) +
    theme_classic() +
    theme(
        text = element_text(family = "Arial", size = 12),
        plot.title = element_text(face = "bold", hjust = 0.5, size = 14),
        axis.title = element_text(face = "bold"),
        legend.position = "bottom",
        legend.title = element_text(face = "bold"),
        panel.grid.major = element_line(color = "gray90", linewidth = 0.2)
    ) +
    scale_x_continuous(breaks = seq(1000, 10000, by = 1000)) + 
    scale_y_continuous(expand = expansion(mult = c(0.05, 0.1))) 
library(ggplot2)

mis_resultados <- data.frame(
  Metodo = rep(c("Directo", "Funcion", "Functor", "Polimorfismo"), each = 4),
  Tamanio = rep(c(5000, 8000, 12000, 15000), times = 4),
  Tiempo_ms = c(
    # Directo
    598.333, 1023, 2335.33, 3572,
    
    # Funcion
    640, 1146.67, 2574.67, 4007.67,
    
    # Functor
    648.667, 1155, 2596, 4049,
    
    # Polimorfismo
    601.333, 1207, 2735, 4158.33
  )
)

colores_primarios <- c(
  "Directo" = "#0000FF", 
  "Funcion" = "#00AA00", 
  "Functor" = "#FFD700", 
  "Polimorfismo" = "#FF0000"
)

ggplot(mis_resultados, aes(x = Tamanio, y = Tiempo_ms, color = Metodo)) +
  geom_point(size = 3, alpha = 0.8) +
  geom_smooth(method = "loess", span = 0.7, se = FALSE, linewidth = 0.7) + 
  labs(
    title = "Comparación de Rendimiento entre Métodos Bubble Sort",
    x = "Tamaño del Vector (elementos)",
    y = "Tiempo de Ejecución (milisegundos)",
    color = "Método"
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
  scale_x_continuous(breaks = seq(5000, 15000, by = 1000)) + 
  scale_y_continuous(expand = expansion(mult = c(0.05, 0.1)))

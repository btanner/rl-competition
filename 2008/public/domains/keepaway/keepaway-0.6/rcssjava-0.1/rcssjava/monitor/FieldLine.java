package rcssjava.monitor;

import java.awt.Color;
import rcssjava.*;

/**
 * Line drawn on field
 * @author Gregory Kuhlmann
 */
public class FieldLine extends FieldShape
{
    private Color color;
    private VecPosition start;
    private VecPosition end;

    public FieldLine( Color color,
		      VecPosition start, VecPosition end,
		      int depth )
    {
        this.color = color;
	this.start = start;
	this.end = end;
	this.depth = depth;
    }

    public void draw( FieldCanvas fc )
    {
	fc.drawLine( start, end, color );
    }
}

#include "precedence_rules.h"

/**
 * @brief Table of precedence rules.
 */
const prec_rule_elem_t precedence_rules[PRECEDENCE_RULE_COUNT][PRECEDENCE_RULE_SIZE] = {
	{{TERM, VAR_ID	}, {NAN,  -1	}, {NAN,  -1	}}, //1.  E -> i
	{{TERM, STR_LIT	}, {NAN,  -1	}, {NAN,  -1	}}, //2.  E -> i
	{{TERM, FLT_LIT	}, {NAN,  -1	}, {NAN,  -1	}}, //3.  E -> i
	{{TERM, INT_LIT	}, {NAN,  -1	}, {NAN,  -1	}}, //4.  E -> i
	{{TERM, LB		}, {EXPR, -1	}, {TERM, RB	}}, //5.  E -> ( E )
	{{EXPR,	-1		}, {TERM, ADD	}, {EXPR, -1	}}, //6.  E -> E + E
	{{EXPR,	-1      }, {TERM, SUB	}, {EXPR, -1	}}, //7.  E -> E - E
	{{EXPR,	-1      }, {TERM, MUL	}, {EXPR, -1	}}, //8.  E -> E * E
	{{EXPR,	-1		}, {TERM, DIV	}, {EXPR, -1	}}, //9.  E -> E / E
	{{EXPR,	-1		}, {TERM, CONCAT}, {EXPR, -1	}}, //10. E -> E . E
	{{EXPR,	-1		}, {TERM, LT	}, {EXPR, -1	}}, //11. E -> E < E
	{{EXPR,	-1		}, {TERM, GT	}, {EXPR, -1	}}, //12. E -> E > E
	{{EXPR,	-1		}, {TERM, LTE	}, {EXPR, -1	}}, //13. E -> E <= E
	{{EXPR,	-1		}, {TERM, GTE	}, {EXPR, -1	}}, //14. E -> E >= E
	{{EXPR,	-1		}, {TERM, EQ	}, {EXPR, -1	}}, //15. E -> E === E
	{{EXPR,	-1		}, {TERM, NEQ	}, {EXPR, -1	}}  //16. E -> E !== E
};
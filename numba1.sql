-- Generate a list of customer purchases, including subtotals for the invoice line numbers. 
-- Subtotals are a derived attribute, calculated by multipying Line_Units by Line_Price).
-- Columns displayed should be xxCus_Code, xxInv_Number, xxP_Description, xxUnits Bought (this is an alias), 
-- xxUnit Price (alias) and xxSubtotal (alias). 


SELECT 
  customer.CUS_CODE AS "Customer Code",
  invoice.INV_NUMBER AS "Invoice Number",
  product.P_DESCRIPT AS "Product Description",
  line.LINE_UNITS AS "Units Bought",
  line.LINE_PRICE AS "Line Price",
  (line.LINE_UNITS * line.LINE_PRICE) AS "Subtotal"
FROM 
  customer
JOIN  
  invoice ON customer.CUS_CODE = invoice.CUS_CODE
JOIN 
  line ON invoice.INV_NUMBER = line.INV_NUMBER
JOIN 
  product ON line.P_CODE = product.P_CODE
ORDER BY 
  customer.CUS_CODE, invoice.INV_NUMBER, product.P_DESCRIPT;
